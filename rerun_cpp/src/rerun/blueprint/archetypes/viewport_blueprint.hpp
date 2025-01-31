// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/cpp/mod.rs
// Based on "crates/store/re_types/definitions/rerun/blueprint/archetypes/viewport_blueprint.fbs".

#pragma once

#include "../../blueprint/components/auto_layout.hpp"
#include "../../blueprint/components/auto_views.hpp"
#include "../../blueprint/components/root_container.hpp"
#include "../../blueprint/components/view_maximized.hpp"
#include "../../blueprint/components/viewer_recommendation_hash.hpp"
#include "../../collection.hpp"
#include "../../component_batch.hpp"
#include "../../component_column.hpp"
#include "../../indicator_component.hpp"
#include "../../result.hpp"

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace rerun::blueprint::archetypes {
    /// **Archetype**: The top-level description of the viewport.
    struct ViewportBlueprint {
        /// The layout of the views
        std::optional<ComponentBatch> root_container;

        /// Show one tab as maximized?
        std::optional<ComponentBatch> maximized;

        /// Whether the viewport layout is determined automatically.
        ///
        /// If `true`, the container layout will be reset whenever a new view is added or removed.
        /// This defaults to `false` and is automatically set to `false` when there is user determined layout.
        std::optional<ComponentBatch> auto_layout;

        /// Whether or not views should be created automatically.
        ///
        /// If `true`, the viewer will only add views that it hasn't considered previously (as identified by `past_viewer_recommendations`)
        /// and which aren't deemed redundant to existing views.
        /// This defaults to `false` and is automatically set to `false` when the user adds views manually in the viewer.
        std::optional<ComponentBatch> auto_views;

        /// Hashes of all recommended views the viewer has already added and that should not be added again.
        ///
        /// This is an internal field and should not be set usually.
        /// If you want the viewer from stopping to add views, you should set `auto_views` to `false`.
        ///
        /// The viewer uses this to determine whether it should keep adding views.
        std::optional<ComponentBatch> past_viewer_recommendations;

      public:
        static constexpr const char IndicatorComponentName[] =
            "rerun.blueprint.components.ViewportBlueprintIndicator";

        /// Indicator component, used to identify the archetype when converting to a list of components.
        using IndicatorComponent = rerun::components::IndicatorComponent<IndicatorComponentName>;
        /// The name of the archetype as used in `ComponentDescriptor`s.
        static constexpr const char ArchetypeName[] =
            "rerun.blueprint.archetypes.ViewportBlueprint";

        /// `ComponentDescriptor` for the `root_container` field.
        static constexpr auto Descriptor_root_container = ComponentDescriptor(
            ArchetypeName, "root_container",
            Loggable<rerun::blueprint::components::RootContainer>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `maximized` field.
        static constexpr auto Descriptor_maximized = ComponentDescriptor(
            ArchetypeName, "maximized",
            Loggable<rerun::blueprint::components::ViewMaximized>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `auto_layout` field.
        static constexpr auto Descriptor_auto_layout = ComponentDescriptor(
            ArchetypeName, "auto_layout",
            Loggable<rerun::blueprint::components::AutoLayout>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `auto_views` field.
        static constexpr auto Descriptor_auto_views = ComponentDescriptor(
            ArchetypeName, "auto_views",
            Loggable<rerun::blueprint::components::AutoViews>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `past_viewer_recommendations` field.
        static constexpr auto Descriptor_past_viewer_recommendations = ComponentDescriptor(
            ArchetypeName, "past_viewer_recommendations",
            Loggable<rerun::blueprint::components::ViewerRecommendationHash>::Descriptor
                .component_name
        );

      public:
        ViewportBlueprint() = default;
        ViewportBlueprint(ViewportBlueprint&& other) = default;
        ViewportBlueprint(const ViewportBlueprint& other) = default;
        ViewportBlueprint& operator=(const ViewportBlueprint& other) = default;
        ViewportBlueprint& operator=(ViewportBlueprint&& other) = default;

        /// Update only some specific fields of a `ViewportBlueprint`.
        static ViewportBlueprint update_fields() {
            return ViewportBlueprint();
        }

        /// Clear all the fields of a `ViewportBlueprint`.
        static ViewportBlueprint clear_fields();

        /// The layout of the views
        ViewportBlueprint with_root_container(
            const rerun::blueprint::components::RootContainer& _root_container
        ) && {
            root_container =
                ComponentBatch::from_loggable(_root_container, Descriptor_root_container)
                    .value_or_throw();
            return std::move(*this);
        }

        /// Show one tab as maximized?
        ViewportBlueprint with_maximized(
            const rerun::blueprint::components::ViewMaximized& _maximized
        ) && {
            maximized =
                ComponentBatch::from_loggable(_maximized, Descriptor_maximized).value_or_throw();
            return std::move(*this);
        }

        /// Whether the viewport layout is determined automatically.
        ///
        /// If `true`, the container layout will be reset whenever a new view is added or removed.
        /// This defaults to `false` and is automatically set to `false` when there is user determined layout.
        ViewportBlueprint with_auto_layout(
            const rerun::blueprint::components::AutoLayout& _auto_layout
        ) && {
            auto_layout = ComponentBatch::from_loggable(_auto_layout, Descriptor_auto_layout)
                              .value_or_throw();
            return std::move(*this);
        }

        /// Whether or not views should be created automatically.
        ///
        /// If `true`, the viewer will only add views that it hasn't considered previously (as identified by `past_viewer_recommendations`)
        /// and which aren't deemed redundant to existing views.
        /// This defaults to `false` and is automatically set to `false` when the user adds views manually in the viewer.
        ViewportBlueprint with_auto_views(const rerun::blueprint::components::AutoViews& _auto_views
        ) && {
            auto_views =
                ComponentBatch::from_loggable(_auto_views, Descriptor_auto_views).value_or_throw();
            return std::move(*this);
        }

        /// Hashes of all recommended views the viewer has already added and that should not be added again.
        ///
        /// This is an internal field and should not be set usually.
        /// If you want the viewer from stopping to add views, you should set `auto_views` to `false`.
        ///
        /// The viewer uses this to determine whether it should keep adding views.
        ViewportBlueprint with_past_viewer_recommendations(
            const Collection<rerun::blueprint::components::ViewerRecommendationHash>&
                _past_viewer_recommendations
        ) && {
            past_viewer_recommendations = ComponentBatch::from_loggable(
                                              _past_viewer_recommendations,
                                              Descriptor_past_viewer_recommendations
            )
                                              .value_or_throw();
            return std::move(*this);
        }

        /// Partitions the component data into multiple sub-batches.
        ///
        /// Specifically, this transforms the existing `ComponentBatch` data into `ComponentColumn`s
        /// instead, via `ComponentColumn::from_batch_with_lengths`.
        ///
        /// This makes it possible to use `RecordingStream::send_columns` to send columnar data directly into Rerun.
        ///
        /// The specified `lengths` must sum to the total length of the component batch.
        Collection<ComponentColumn> columns(const Collection<uint32_t>& lengths_);

        /// Partitions the component data into unit-length sub-batches.
        ///
        /// This is semantically similar to calling `columns` with `std::vector<uint32_t>(n, 1)`,
        /// where `n` is automatically guessed.
        Collection<ComponentColumn> columns();
    };

} // namespace rerun::blueprint::archetypes

namespace rerun {
    /// \private
    template <typename T>
    struct AsComponents;

    /// \private
    template <>
    struct AsComponents<blueprint::archetypes::ViewportBlueprint> {
        /// Serialize all set component batches.
        static Result<Collection<ComponentBatch>> as_batches(
            const blueprint::archetypes::ViewportBlueprint& archetype
        );
    };
} // namespace rerun
