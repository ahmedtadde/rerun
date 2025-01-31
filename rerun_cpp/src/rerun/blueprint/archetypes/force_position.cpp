// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/cpp/mod.rs
// Based on "crates/store/re_types/definitions/rerun/blueprint/archetypes/force_position.fbs".

#include "force_position.hpp"

#include "../../collection_adapter_builtins.hpp"

namespace rerun::blueprint::archetypes {
    ForcePosition ForcePosition::clear_fields() {
        auto archetype = ForcePosition();
        archetype.enabled =
            ComponentBatch::empty<rerun::blueprint::components::Enabled>(Descriptor_enabled)
                .value_or_throw();
        archetype.strength =
            ComponentBatch::empty<rerun::blueprint::components::ForceStrength>(Descriptor_strength)
                .value_or_throw();
        archetype.position =
            ComponentBatch::empty<rerun::components::Position2D>(Descriptor_position)
                .value_or_throw();
        return archetype;
    }

    Collection<ComponentColumn> ForcePosition::columns(const Collection<uint32_t>& lengths_) {
        std::vector<ComponentColumn> columns;
        columns.reserve(4);
        if (enabled.has_value()) {
            columns.push_back(
                ComponentColumn::from_batch_with_lengths(enabled.value(), lengths_).value_or_throw()
            );
        }
        if (strength.has_value()) {
            columns.push_back(ComponentColumn::from_batch_with_lengths(strength.value(), lengths_)
                                  .value_or_throw());
        }
        if (position.has_value()) {
            columns.push_back(ComponentColumn::from_batch_with_lengths(position.value(), lengths_)
                                  .value_or_throw());
        }
        columns.push_back(
            ComponentColumn::from_indicators<ForcePosition>(static_cast<uint32_t>(lengths_.size()))
                .value_or_throw()
        );
        return columns;
    }

    Collection<ComponentColumn> ForcePosition::columns() {
        if (enabled.has_value()) {
            return columns(std::vector<uint32_t>(enabled.value().length(), 1));
        }
        if (strength.has_value()) {
            return columns(std::vector<uint32_t>(strength.value().length(), 1));
        }
        if (position.has_value()) {
            return columns(std::vector<uint32_t>(position.value().length(), 1));
        }
        return Collection<ComponentColumn>();
    }
} // namespace rerun::blueprint::archetypes

namespace rerun {

    Result<Collection<ComponentBatch>>
        AsComponents<blueprint::archetypes::ForcePosition>::as_batches(
            const blueprint::archetypes::ForcePosition& archetype
        ) {
        using namespace blueprint::archetypes;
        std::vector<ComponentBatch> cells;
        cells.reserve(4);

        if (archetype.enabled.has_value()) {
            cells.push_back(archetype.enabled.value());
        }
        if (archetype.strength.has_value()) {
            cells.push_back(archetype.strength.value());
        }
        if (archetype.position.has_value()) {
            cells.push_back(archetype.position.value());
        }
        {
            auto result = ComponentBatch::from_indicator<ForcePosition>();
            RR_RETURN_NOT_OK(result.error);
            cells.emplace_back(std::move(result.value));
        }

        return rerun::take_ownership(std::move(cells));
    }
} // namespace rerun
