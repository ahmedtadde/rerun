// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/cpp/mod.rs
// Based on "crates/store/re_types/definitions/rerun/blueprint/archetypes/visualizer_overrides.fbs".

#include "visualizer_overrides.hpp"

#include "../../collection_adapter_builtins.hpp"

namespace rerun::blueprint::archetypes {
    VisualizerOverrides VisualizerOverrides::clear_fields() {
        auto archetype = VisualizerOverrides();
        archetype.ranges = ComponentBatch::empty<rerun::blueprint::components::VisualizerOverride>(
                               Descriptor_ranges
        )
                               .value_or_throw();
        return archetype;
    }

    Collection<ComponentColumn> VisualizerOverrides::columns(const Collection<uint32_t>& lengths_) {
        std::vector<ComponentColumn> columns;
        columns.reserve(2);
        if (ranges.has_value()) {
            columns.push_back(ranges.value().partitioned(lengths_).value_or_throw());
        }
        columns.push_back(ComponentColumn::from_indicators<VisualizerOverrides>(
                              static_cast<uint32_t>(lengths_.size())
        )
                              .value_or_throw());
        return columns;
    }

    Collection<ComponentColumn> VisualizerOverrides::columns() {
        if (ranges.has_value()) {
            return columns(std::vector<uint32_t>(ranges.value().length(), 1));
        }
        return Collection<ComponentColumn>();
    }
} // namespace rerun::blueprint::archetypes

namespace rerun {

    Result<Collection<ComponentBatch>>
        AsComponents<blueprint::archetypes::VisualizerOverrides>::as_batches(
            const blueprint::archetypes::VisualizerOverrides& archetype
        ) {
        using namespace blueprint::archetypes;
        std::vector<ComponentBatch> cells;
        cells.reserve(2);

        if (archetype.ranges.has_value()) {
            cells.push_back(archetype.ranges.value());
        }
        {
            auto result = ComponentBatch::from_indicator<VisualizerOverrides>();
            RR_RETURN_NOT_OK(result.error);
            cells.emplace_back(std::move(result.value));
        }

        return rerun::take_ownership(std::move(cells));
    }
} // namespace rerun
