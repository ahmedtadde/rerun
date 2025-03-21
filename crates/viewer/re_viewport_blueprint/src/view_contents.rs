use std::sync::Arc;

use nohash_hasher::{IntMap, IntSet};
use parking_lot::Mutex;
use slotmap::SlotMap;
use smallvec::SmallVec;

use re_entity_db::{external::re_chunk_store::LatestAtQuery, EntityDb, EntityTree};
use re_log_types::{
    path::RuleEffect, EntityPath, EntityPathFilter, EntityPathSubs, ResolvedEntityPathFilter,
    ResolvedEntityPathRule, Timeline,
};
use re_types::blueprint::components::VisualizerOverride;
use re_types::{
    blueprint::{
        archetypes as blueprint_archetypes, components as blueprint_components,
        components::QueryExpression,
    },
    Archetype as _, ViewClassIdentifier,
};
use re_types_core::ComponentName;
use re_viewer_context::{
    DataQueryResult, DataResult, DataResultHandle, DataResultNode, DataResultTree,
    IndicatedEntities, MaybeVisualizableEntities, OverridePath, PerVisualizer, PropertyOverrides,
    QueryRange, ViewClassRegistry, ViewId, ViewStates, ViewerContext, VisualizableEntities,
};

use crate::{ViewBlueprint, ViewProperty};

/// Data to be added to a view, built from a [`blueprint_archetypes::ViewContents`].
///
/// During execution, it will walk an [`EntityTree`] and return a [`DataResultTree`]
/// containing any entities that match a [`ResolvedEntityPathFilter`].
///
/// Note: [`ViewContents`] doesn't implement Clone because it depends on its parent's [`ViewId`]
/// used for identifying the path of its data in the blueprint store. It's ambiguous
/// whether the intent is for a clone to write to the same place.
///
/// If you want a new view otherwise identical to an existing one, use
/// [`ViewBlueprint::duplicate`].
#[derive(Clone, Debug)]
pub struct ViewContents {
    view_id: ViewId,

    view_class_identifier: ViewClassIdentifier,

    /// Deserialized entity path filter.
    ///
    /// Consider this read-only.
    entity_path_filter: ResolvedEntityPathFilter,

    /// Update entity path filter.
    ///
    /// Mutations go to this value and should be saved to the blueprint store when they occur.
    new_entity_path_filter: Arc<Mutex<ResolvedEntityPathFilter>>,
}

impl ViewContents {
    pub fn is_equivalent(&self, other: &Self) -> bool {
        self.view_class_identifier.eq(&other.view_class_identifier)
            && self.entity_path_filter.eq(&other.entity_path_filter)
    }

    /// Checks whether the results of this query "fully contains" the results of another query.
    ///
    /// If this returns `true` then the [`DataQueryResult`] returned by this query should always
    /// contain any [`EntityPath`] that would be included in the results of the other query.
    ///
    /// This is a conservative estimate, and may return `false` in situations where the
    /// query does in fact cover the other query. However, it should never return `true`
    /// in a case where the other query would not be fully covered.
    pub fn entity_path_filter_is_superset_of(&self, other: &Self) -> bool {
        // A query can't fully contain another if their view classes don't match
        if self.view_class_identifier != other.view_class_identifier {
            return false;
        }

        // Anything included by the other query is also included by this query
        self.entity_path_filter
            .is_superset_of(&other.entity_path_filter)
    }
}

impl ViewContents {
    const INDIVIDUAL_OVERRIDES_PREFIX: &'static str = "individual_overrides";
    const RECURSIVE_OVERRIDES_PREFIX: &'static str = "recursive_overrides";

    /// Creates a new [`ViewContents`].
    ///
    /// This [`ViewContents`] is ephemeral. It must be saved by calling
    /// `save_to_blueprint_store` on the enclosing `ViewBlueprint`.
    pub fn new(
        view_id: ViewId,
        view_class_identifier: ViewClassIdentifier,
        entity_path_filter: ResolvedEntityPathFilter,
    ) -> Self {
        let new_entity_path_filter = Arc::new(Mutex::new(entity_path_filter.clone()));

        Self {
            view_id,
            view_class_identifier,
            entity_path_filter,
            new_entity_path_filter,
        }
    }

    /// Entity path for a given view id.
    fn blueprint_entity_path_for_id(id: ViewId) -> EntityPath {
        // Don't use `entity_path_for_view_property` here because this will do a search in the future,
        // thus needing the entity tree.
        id.as_entity_path().join(&EntityPath::from_single_string(
            blueprint_archetypes::ViewContents::name().short_name(),
        ))
    }

    /// Computes the override path for a given entity in a given view.
    pub fn override_path_for_entity(id: ViewId, entity_path: &EntityPath) -> EntityPath {
        Self::blueprint_entity_path_for_id(id)
            .join(&EntityPath::from_single_string(
                Self::INDIVIDUAL_OVERRIDES_PREFIX,
            ))
            .join(entity_path)
    }

    /// Computes the recursive override path for a given entity in a given view.
    pub fn recursive_override_path_for_entity(id: ViewId, entity_path: &EntityPath) -> EntityPath {
        Self::blueprint_entity_path_for_id(id)
            .join(&EntityPath::from_single_string(
                Self::RECURSIVE_OVERRIDES_PREFIX,
            ))
            .join(entity_path)
    }

    /// Attempt to load a [`ViewContents`] from the blueprint store.
    pub fn from_db_or_default(
        view_id: ViewId,
        blueprint_db: &EntityDb,
        query: &LatestAtQuery,
        view_class_identifier: ViewClassIdentifier,
        subst_env: &EntityPathSubs,
    ) -> Self {
        let property = ViewProperty::from_archetype::<blueprint_archetypes::ViewContents>(
            blueprint_db,
            query,
            view_id,
        );
        let expressions = match property.component_array_or_empty::<QueryExpression>() {
            Ok(expressions) => expressions,

            Err(err) => {
                re_log::warn_once!(
                    "Failed to load ViewContents for {:?} from blueprint store at {:?}: {}",
                    view_id,
                    property.blueprint_store_path,
                    err
                );
                Default::default()
            }
        };
        let query = expressions.iter().map(|qe| qe.0.as_str());
        let entity_path_filter =
            EntityPathFilter::from_query_expressions(query).resolve_forgiving(subst_env);

        let new_entity_path_filter = Arc::new(Mutex::new(entity_path_filter.clone()));

        Self {
            view_id,
            view_class_identifier,
            entity_path_filter,
            new_entity_path_filter,
        }
    }

    /// Persist the entire [`ViewContents`] to the blueprint store.
    ///
    /// This only needs to be called if the [`ViewContents`] was created with [`Self::new`].
    ///
    /// Otherwise, incremental calls to `set_` functions will write just the necessary component
    /// update directly to the store.
    pub fn save_to_blueprint_store(&self, ctx: &ViewerContext<'_>) {
        self.save_entity_path_filter_to_blueprint(ctx);
    }

    pub fn entity_path_filter(&self) -> &ResolvedEntityPathFilter {
        &self.entity_path_filter
    }

    /// Sets the entity path filter to the provided one.
    pub fn set_entity_path_filter(
        &self,
        ctx: &ViewerContext<'_>,
        entity_path_filter: ResolvedEntityPathFilter,
    ) {
        *self.new_entity_path_filter.lock() = entity_path_filter;
        self.save_entity_path_filter_to_blueprint(ctx);
    }

    /// Perform arbitrary mutation on the entity path filter.
    ///
    /// Using this method avoids triggering multiple writes to the blueprint store.
    pub fn mutate_entity_path_filter(
        &self,
        ctx: &ViewerContext<'_>,
        f: impl FnOnce(&mut ResolvedEntityPathFilter),
    ) {
        f(&mut self.new_entity_path_filter.lock());
        self.save_entity_path_filter_to_blueprint(ctx);
    }

    /// Remove a subtree and any existing rules that it would match.
    ///
    /// Because most-specific matches win, if we only add a subtree exclusion
    /// it can still be overridden by existing inclusions. This method ensures
    /// that not only do we add a subtree exclusion, but clear out any existing
    /// inclusions or (now redundant) exclusions that would match the subtree.
    pub fn remove_subtree_and_matching_rules(&self, ctx: &ViewerContext<'_>, path: EntityPath) {
        self.new_entity_path_filter
            .lock()
            .remove_subtree_and_matching_rules(path);

        self.save_entity_path_filter_to_blueprint(ctx);
    }

    /// Directly add an exclusion rule to the [`EntityPathFilter`].
    ///
    /// This is a direct modification of the filter and will not do any simplification
    /// related to overlapping or conflicting rules.
    ///
    /// If you are trying to remove an entire subtree, prefer using [`Self::remove_subtree_and_matching_rules`].
    pub fn raw_add_entity_exclusion(&self, ctx: &ViewerContext<'_>, rule: ResolvedEntityPathRule) {
        self.new_entity_path_filter
            .lock()
            .add_rule(RuleEffect::Exclude, rule);

        self.save_entity_path_filter_to_blueprint(ctx);
    }

    /// Directly add an inclusion rule to the [`EntityPathFilter`].
    ///
    /// This is a direct modification of the filter and will not do any simplification
    /// related to overlapping or conflicting rules.
    pub fn raw_add_entity_inclusion(&self, ctx: &ViewerContext<'_>, rule: ResolvedEntityPathRule) {
        self.new_entity_path_filter
            .lock()
            .add_rule(RuleEffect::Include, rule);

        self.save_entity_path_filter_to_blueprint(ctx);
    }

    /// Remove rules for a given entity.
    pub fn remove_filter_rule_for(&self, ctx: &ViewerContext<'_>, ent_path: &EntityPath) {
        self.new_entity_path_filter.lock().remove_rule_for(ent_path);

        self.save_entity_path_filter_to_blueprint(ctx);
    }

    /// Save the entity path filter.
    fn save_entity_path_filter_to_blueprint(&self, ctx: &ViewerContext<'_>) {
        ViewProperty::from_archetype::<blueprint_archetypes::ViewContents>(
            ctx.blueprint_db(),
            ctx.blueprint_query,
            self.view_id,
        )
        .save_blueprint_component(
            ctx,
            &self
                .new_entity_path_filter
                .lock()
                .iter_unresolved_expressions()
                .map(|s| blueprint_components::QueryExpression(s.into()))
                .collect::<Vec<_>>(),
        );
    }

    /// Build up the initial [`DataQueryResult`] for this [`ViewContents`]
    ///
    /// Note that this result will not have any resolved [`PropertyOverrides`]. Those can
    /// be added by separately calling `DataQueryPropertyResolver::update_overrides` on
    /// the result.
    pub fn execute_query(
        &self,
        ctx: &re_viewer_context::StoreContext<'_>,
        view_class_registry: &re_viewer_context::ViewClassRegistry,
        blueprint_query: &LatestAtQuery,
        view_id: ViewId,
        visualizable_entities_for_visualizer_systems: &PerVisualizer<VisualizableEntities>,
    ) -> DataQueryResult {
        re_tracing::profile_function!();

        let mut data_results = SlotMap::<DataResultHandle, DataResultNode>::default();

        let executor = QueryExpressionEvaluator {
            visualizable_entities_for_visualizer_systems,
            entity_path_filter: self.entity_path_filter.clone(),
            recursive_override_base_path: Self::recursive_override_path_for_entity(
                view_id,
                &EntityPath::root(),
            ),
            individual_override_base_path: Self::override_path_for_entity(
                view_id,
                &EntityPath::root(),
            ),
        };

        let mut num_matching_entities = 0;
        let mut num_visualized_entities = 0;
        let root_handle = {
            re_tracing::profile_scope!("add_entity_tree_to_data_results_recursive");
            executor.add_entity_tree_to_data_results_recursive(
                ctx.recording.tree(),
                &mut data_results,
                &mut num_matching_entities,
                &mut num_visualized_entities,
            )
        };

        // Query defaults for all the components that any visualizer in this view is interested in.
        let component_defaults = {
            re_tracing::profile_scope!("component_defaults");

            let visualizer_collection =
                view_class_registry.new_visualizer_collection(self.view_class_identifier);

            // Figure out which components are relevant.
            let mut components_for_defaults = IntSet::default();
            for (visualizer, entities) in visualizable_entities_for_visualizer_systems.iter() {
                if entities.is_empty() {
                    continue;
                }
                let Ok(visualizer) = visualizer_collection.get_by_identifier(*visualizer) else {
                    continue;
                };
                components_for_defaults.extend(visualizer.visualizer_query_info().queried.iter());
            }

            ctx.blueprint.latest_at(
                blueprint_query,
                &ViewBlueprint::defaults_path(view_id),
                components_for_defaults,
            )
        };

        DataQueryResult {
            tree: DataResultTree::new(data_results, root_handle),
            num_matching_entities,
            num_visualized_entities,
            component_defaults,
        }
    }
}

/// Helper struct for executing the query from [`ViewContents`]
///
/// This restructures the [`QueryExpression`] into several sets that are
/// used to efficiently determine if we should continue the walk or switch
/// to a pure recursive evaluation.
struct QueryExpressionEvaluator<'a> {
    visualizable_entities_for_visualizer_systems: &'a PerVisualizer<VisualizableEntities>,
    entity_path_filter: ResolvedEntityPathFilter,
    recursive_override_base_path: EntityPath,
    individual_override_base_path: EntityPath,
}

impl QueryExpressionEvaluator<'_> {
    fn add_entity_tree_to_data_results_recursive(
        &self,
        tree: &EntityTree,
        data_results: &mut SlotMap<DataResultHandle, DataResultNode>,
        num_matching_entities: &mut usize,
        num_visualized_entities: &mut usize,
    ) -> Option<DataResultHandle> {
        // Early-out optimization
        if !self
            .entity_path_filter
            .is_anything_in_subtree_included(&tree.path)
        {
            return None;
        }

        // TODO(jleibs): If this space is disconnected, we should terminate here

        let entity_path = &tree.path;

        let matches_filter = self.entity_path_filter.matches(entity_path);
        *num_matching_entities += matches_filter as usize;

        // This list will be updated below during `update_overrides_recursive` by calling `choose_default_visualizers`
        // on the view.
        let visualizers: SmallVec<[_; 4]> = if matches_filter {
            self.visualizable_entities_for_visualizer_systems
                .iter()
                .filter_map(|(visualizer, ents)| ents.contains(entity_path).then_some(*visualizer))
                .collect()
        } else {
            Default::default()
        };
        *num_visualized_entities += !visualizers.is_empty() as usize;

        let children: SmallVec<[_; 4]> = tree
            .children
            .values()
            .filter_map(|subtree| {
                self.add_entity_tree_to_data_results_recursive(
                    subtree,
                    data_results,
                    num_matching_entities,
                    num_visualized_entities,
                )
            })
            .collect();

        // Ignore empty nodes.
        // Since we recurse downwards, this prunes any branches that don't have anything to contribute to the scene
        // and aren't directly included.
        let exact_included = self.entity_path_filter.matches_exactly(entity_path);
        if exact_included || !children.is_empty() || !visualizers.is_empty() {
            Some(data_results.insert(DataResultNode {
                data_result: DataResult {
                    entity_path: entity_path.clone(),
                    visualizers,
                    tree_prefix_only: !matches_filter,
                    property_overrides: PropertyOverrides {
                        resolved_component_overrides: IntMap::default(), // Determined later during `update_overrides_recursive`.
                        recursive_override_path:
                            self.recursive_override_base_path.join(entity_path),
                        override_path: self.individual_override_base_path.join(entity_path),
                        query_range: QueryRange::default(), // Determined later during `update_overrides_recursive`.
                    },
                },
                children,
            }))
        } else {
            None
        }
    }
}

pub struct DataQueryPropertyResolver<'a> {
    view_class_registry: &'a re_viewer_context::ViewClassRegistry,
    view: &'a ViewBlueprint,
    individual_override_root: EntityPath,
    recursive_override_root: EntityPath,
    maybe_visualizable_entities_per_visualizer: &'a PerVisualizer<MaybeVisualizableEntities>,
    visualizable_entities_per_visualizer: &'a PerVisualizer<VisualizableEntities>,
    indicated_entities_per_visualizer: &'a PerVisualizer<IndicatedEntities>,
}

impl<'a> DataQueryPropertyResolver<'a> {
    pub fn new(
        view: &'a ViewBlueprint,
        view_class_registry: &'a re_viewer_context::ViewClassRegistry,
        maybe_visualizable_entities_per_visualizer: &'a PerVisualizer<MaybeVisualizableEntities>,
        visualizable_entities_per_visualizer: &'a PerVisualizer<VisualizableEntities>,
        indicated_entities_per_visualizer: &'a PerVisualizer<IndicatedEntities>,
    ) -> Self {
        let individual_override_root =
            ViewContents::override_path_for_entity(view.id, &EntityPath::root());
        let recursive_override_root =
            ViewContents::recursive_override_path_for_entity(view.id, &EntityPath::root());

        Self {
            view_class_registry,
            view,
            individual_override_root,
            recursive_override_root,
            maybe_visualizable_entities_per_visualizer,
            visualizable_entities_per_visualizer,
            indicated_entities_per_visualizer,
        }
    }

    /// Recursively walk the [`DataResultTree`] and update the [`PropertyOverrides`] for each node.
    ///
    /// This will accumulate the recursive properties at each step down the tree, and then merge
    /// with individual overrides on each step.
    #[allow(clippy::too_many_arguments)]
    fn update_overrides_recursive(
        &self,
        blueprint: &EntityDb,
        blueprint_query: &LatestAtQuery,
        active_timeline: &Timeline,
        query_result: &mut DataQueryResult,
        default_query_range: &QueryRange,
        recursive_property_overrides: &IntMap<ComponentName, OverridePath>,
        handle: DataResultHandle,
    ) {
        let blueprint_engine = blueprint.storage_engine();

        if let Some((child_handles, recursive_property_overrides)) =
            query_result.tree.lookup_node_mut(handle).map(|node| {
                let individual_override_path = self
                    .individual_override_root
                    .join(&node.data_result.entity_path);
                let recursive_override_path = self
                    .recursive_override_root
                    .join(&node.data_result.entity_path);

                // Update visualizers from overrides.
                if !node.data_result.visualizers.is_empty() {
                    // If the user has overridden the visualizers, update which visualizers are used.
                    let viz_override_query = blueprint.latest_at(
                        blueprint_query,
                        &individual_override_path,
                        // TODO(andreas): Use tags from `VisualizerOverrides`.
                        [VisualizerOverride::name()],
                    );
                    let viz_overrides = viz_override_query
                        .get_required(&VisualizerOverride::name())
                        .map(|viz_override| {
                            viz_override.iter_slices::<String>(VisualizerOverride::name())
                        });
                    if let Ok(viz_overrides) = viz_overrides {
                        node.data_result.visualizers =
                            viz_overrides.flatten().map(|s| s.as_str().into()).collect();
                    } else {
                        // Otherwise ask the `ViewClass` to choose.
                        node.data_result.visualizers = self
                            .view
                            .class(self.view_class_registry)
                            .choose_default_visualizers(
                                &node.data_result.entity_path,
                                self.maybe_visualizable_entities_per_visualizer,
                                self.visualizable_entities_per_visualizer,
                                self.indicated_entities_per_visualizer,
                            );
                    }
                }

                // First, gather recursive overrides. Previous recursive overrides are the base for the next.
                // We assume that most of the time there's no new recursive overrides, so clone the map lazily.
                let mut recursive_property_overrides =
                    std::borrow::Cow::Borrowed(recursive_property_overrides);
                if let Some(recursive_override_subtree) =
                    blueprint.tree().subtree(&recursive_override_path)
                {
                    for component_name in blueprint_engine
                        .store()
                        .all_components_for_entity(&recursive_override_subtree.path)
                        .unwrap_or_default()
                    {
                        if let Some(component_data) = blueprint
                            .storage_engine()
                            .cache()
                            .latest_at(blueprint_query, &recursive_override_path, [component_name])
                            .component_batch_raw(&component_name)
                        {
                            if !component_data.is_empty() {
                                recursive_property_overrides.to_mut().insert(
                                    component_name,
                                    OverridePath::blueprint_path(recursive_override_path.clone()),
                                );
                            }
                        }
                    }
                }

                // Then, gather individual overrides - these may override the recursive ones again,
                // but recursive overrides are still inherited to children.
                let resolved_component_overrides = &mut node
                    .data_result
                    .property_overrides
                    .resolved_component_overrides;
                *resolved_component_overrides = (*recursive_property_overrides).clone();
                if let Some(individual_override_subtree) =
                    blueprint.tree().subtree(&individual_override_path)
                {
                    for component_name in blueprint_engine
                        .store()
                        .all_components_for_entity(&individual_override_subtree.path)
                        .unwrap_or_default()
                    {
                        if let Some(component_data) = blueprint
                            .storage_engine()
                            .cache()
                            .latest_at(blueprint_query, &individual_override_path, [component_name])
                            .component_batch_raw(&component_name)
                        {
                            if !component_data.is_empty() {
                                resolved_component_overrides.insert(
                                    component_name,
                                    OverridePath::blueprint_path(individual_override_path.clone()),
                                );
                            }
                        }
                    }
                }

                // Figure out relevant visual time range.
                use re_types::Component as _;
                let latest_at_results = blueprint.latest_at(
                    blueprint_query,
                    &individual_override_path,
                    std::iter::once(blueprint_components::VisibleTimeRange::name()),
                );
                let visible_time_ranges =
                    latest_at_results.component_batch::<blueprint_components::VisibleTimeRange>();
                let time_range = visible_time_ranges.as_ref().and_then(|ranges| {
                    ranges
                        .iter()
                        .find(|range| range.timeline.as_str() == active_timeline.name().as_str())
                });
                node.data_result.property_overrides.query_range = time_range.map_or_else(
                    || default_query_range.clone(),
                    |time_range| QueryRange::TimeRange(time_range.0.range.clone()),
                );

                (node.children.clone(), recursive_property_overrides)
            })
        {
            for child in child_handles {
                self.update_overrides_recursive(
                    blueprint,
                    blueprint_query,
                    active_timeline,
                    query_result,
                    default_query_range,
                    &recursive_property_overrides,
                    child,
                );
            }
        }
    }

    /// Recursively walk the [`DataResultTree`] and update the [`PropertyOverrides`] for each node.
    pub fn update_overrides(
        &self,
        blueprint: &EntityDb,
        blueprint_query: &LatestAtQuery,
        active_timeline: &Timeline,
        view_class_registry: &ViewClassRegistry,
        query_result: &mut DataQueryResult,
        view_states: &mut ViewStates,
    ) {
        // This is called very frequently, don't put a profile scope here.

        if let Some(root) = query_result.tree.root_handle() {
            let recursive_property_overrides = Default::default();

            let class = self.view.class(view_class_registry);
            let view_state = view_states.get_mut_or_create(self.view.id, class);

            let default_query_range = self.view.query_range(
                blueprint,
                blueprint_query,
                active_timeline,
                view_class_registry,
                view_state,
            );

            self.update_overrides_recursive(
                blueprint,
                blueprint_query,
                active_timeline,
                query_result,
                &default_query_range,
                &recursive_property_overrides,
                root,
            );
        }
    }
}

#[cfg(test)]
mod tests {
    use std::sync::Arc;

    use re_chunk::{Chunk, RowId};
    use re_entity_db::EntityDb;
    use re_log_types::{example_components::MyPoint, StoreId, TimePoint, Timeline};
    use re_viewer_context::{blueprint_timeline, StoreContext, StoreHub, VisualizableEntities};

    use super::*;

    #[test]
    fn test_query_results() {
        let space_env = EntityPathSubs::empty();

        let mut recording = EntityDb::new(StoreId::random(re_log_types::StoreKind::Recording));
        let blueprint = EntityDb::new(StoreId::random(re_log_types::StoreKind::Blueprint));

        let timeline_frame = Timeline::new_sequence("frame");
        let timepoint = TimePoint::from_iter([(timeline_frame, 10)]);
        let view_class_registry = ViewClassRegistry::default();

        // Set up a store DB with some entities
        for entity_path in ["parent", "parent/skipped/child1", "parent/skipped/child2"] {
            let row_id = RowId::new();
            let point = MyPoint::new(1.0, 2.0);
            let chunk = Chunk::builder(entity_path.into())
                .with_component_batch(row_id, timepoint.clone(), &[point] as _)
                .build()
                .unwrap();

            recording.add_chunk(&Arc::new(chunk)).unwrap();
        }

        let mut visualizable_entities_for_visualizer_systems =
            PerVisualizer::<VisualizableEntities>::default();

        visualizable_entities_for_visualizer_systems
            .0
            .entry("Points3D".into())
            .or_insert_with(|| {
                VisualizableEntities(
                    [
                        EntityPath::from("parent"),
                        EntityPath::from("parent/skipped/child1"),
                    ]
                    .into_iter()
                    .collect(),
                )
            });

        let ctx = StoreContext {
            app_id: re_log_types::ApplicationId::unknown(),
            blueprint: &blueprint,
            default_blueprint: None,
            recording: &recording,
            bundle: &Default::default(),
            caches: &Default::default(),
            hub: &StoreHub::test_hub(),
            should_enable_heuristics: false,
        };

        struct Scenario {
            filter: &'static str,
            outputs: Vec<&'static str>,
        }

        let scenarios: Vec<Scenario> = vec![
            Scenario {
                filter: "+ /**",
                outputs: vec![
                    "/**",
                    "/parent",
                    "/parent/skipped",
                    "/parent/skipped/child1", // Only child 1 has visualizers
                ],
            },
            Scenario {
                filter: "+ parent/skipped/**",
                outputs: vec![
                    "/**",
                    "/parent/**", // Only included because is a prefix
                    "/parent/skipped",
                    "/parent/skipped/child1", // Only child 1 has visualizers
                ],
            },
            Scenario {
                filter: r"+ parent
                          + parent/skipped/child2",
                outputs: vec![
                    "/**", // Trivial intermediate group -- could be collapsed
                    "/parent",
                    "/parent/skipped/**", // Trivial intermediate group -- could be collapsed
                    "/parent/skipped/child2",
                ],
            },
            Scenario {
                filter: r"+ parent/skipped
                          + parent/skipped/child2
                          + parent/**",
                outputs: vec![
                    "/**",
                    "/parent",
                    "/parent/skipped",        // Included because an exact match
                    "/parent/skipped/child1", // Included because an exact match
                    "/parent/skipped/child2",
                ],
            },
            Scenario {
                filter: r"+ parent/skipped
                          + parent/skipped/child2
                          + parent/**
                          - parent",
                outputs: vec![
                    "/**",
                    "/parent/**",             // Parent leaf has been excluded
                    "/parent/skipped",        // Included because an exact match
                    "/parent/skipped/child1", // Included because an exact match
                    "/parent/skipped/child2",
                ],
            },
            Scenario {
                filter: r"+ parent/**
                          - parent/skipped/**",
                outputs: vec!["/**", "/parent"], // None of the children are hit since excluded
            },
            Scenario {
                filter: r"+ parent/**
                          + parent/skipped/child2
                          - parent/skipped/child1",
                outputs: vec![
                    "/**",
                    "/parent",
                    "/parent/skipped",
                    "/parent/skipped/child2", // No child1 since skipped.
                ],
            },
            Scenario {
                filter: r"+ not/found",
                // TODO(jleibs): Making this work requires merging the EntityTree walk with a minimal-coverage ExactMatchTree walk
                // not crucial for now until we expose a free-form UI for entering paths.
                // vec!["/**", "not/**", "not/found"]),
                outputs: vec![],
            },
        ];

        for (i, Scenario { filter, outputs }) in scenarios.into_iter().enumerate() {
            let view_id = ViewId::random();
            let contents = ViewContents::new(
                view_id,
                "3D".into(),
                EntityPathFilter::parse_forgiving(filter).resolve_forgiving(&space_env),
            );

            let query_result = contents.execute_query(
                &ctx,
                &view_class_registry,
                &LatestAtQuery::latest(blueprint_timeline()),
                view_id,
                &visualizable_entities_for_visualizer_systems,
            );

            let mut visited = vec![];
            query_result.tree.visit(&mut |node| {
                let result = &node.data_result;
                if result.entity_path == EntityPath::root() {
                    visited.push("/**".to_owned());
                } else if result.tree_prefix_only {
                    visited.push(format!("{}/**", result.entity_path));
                    assert!(result.visualizers.is_empty());
                } else {
                    visited.push(result.entity_path.to_string());
                }
                true
            });

            assert_eq!(visited, outputs, "Scenario {i}, filter: {filter}");
        }
    }
}
