// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/cpp/mod.rs
// Based on "crates/store/re_types/definitions/rerun/blueprint/archetypes/map_background.fbs".

#include "map_background.hpp"

#include "../../collection_adapter_builtins.hpp"

namespace rerun::blueprint::archetypes {
    MapBackground MapBackground::clear_fields() {
        auto archetype = MapBackground();
        archetype.provider =
            ComponentBatch::empty<rerun::blueprint::components::MapProvider>(Descriptor_provider)
                .value_or_throw();
        return archetype;
    }

    Collection<ComponentColumn> MapBackground::columns(const Collection<uint32_t>& lengths_) {
        std::vector<ComponentColumn> columns;
        columns.reserve(2);
        if (provider.has_value()) {
            columns.push_back(ComponentColumn::from_batch_with_lengths(provider.value(), lengths_)
                                  .value_or_throw());
        }
        columns.push_back(
            ComponentColumn::from_indicators<MapBackground>(static_cast<uint32_t>(lengths_.size()))
                .value_or_throw()
        );
        return columns;
    }

    Collection<ComponentColumn> MapBackground::columns() {
        if (provider.has_value()) {
            return columns(std::vector<uint32_t>(provider.value().length(), 1));
        }
        return Collection<ComponentColumn>();
    }
} // namespace rerun::blueprint::archetypes

namespace rerun {

    Result<std::vector<ComponentBatch>>
        AsComponents<blueprint::archetypes::MapBackground>::serialize(
            const blueprint::archetypes::MapBackground& archetype
        ) {
        using namespace blueprint::archetypes;
        std::vector<ComponentBatch> cells;
        cells.reserve(2);

        if (archetype.provider.has_value()) {
            cells.push_back(archetype.provider.value());
        }
        {
            auto indicator = MapBackground::IndicatorComponent();
            auto result = ComponentBatch::from_loggable(indicator);
            RR_RETURN_NOT_OK(result.error);
            cells.emplace_back(std::move(result.value));
        }

        return cells;
    }
} // namespace rerun
