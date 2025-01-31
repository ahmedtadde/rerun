// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/cpp/mod.rs
// Based on "crates/store/re_types/definitions/rerun/blueprint/archetypes/tensor_view_fit.fbs".

#include "tensor_view_fit.hpp"

#include "../../collection_adapter_builtins.hpp"

namespace rerun::blueprint::archetypes {
    TensorViewFit TensorViewFit::clear_fields() {
        auto archetype = TensorViewFit();
        archetype.scaling =
            ComponentBatch::empty<rerun::blueprint::components::ViewFit>(Descriptor_scaling)
                .value_or_throw();
        return archetype;
    }

    Collection<ComponentColumn> TensorViewFit::columns(const Collection<uint32_t>& lengths_) {
        std::vector<ComponentColumn> columns;
        columns.reserve(2);
        if (scaling.has_value()) {
            columns.push_back(
                ComponentColumn::from_batch_with_lengths(scaling.value(), lengths_).value_or_throw()
            );
        }
        columns.push_back(
            ComponentColumn::from_indicators<TensorViewFit>(static_cast<uint32_t>(lengths_.size()))
                .value_or_throw()
        );
        return columns;
    }

    Collection<ComponentColumn> TensorViewFit::columns() {
        if (scaling.has_value()) {
            return columns(std::vector<uint32_t>(scaling.value().length(), 1));
        }
        return Collection<ComponentColumn>();
    }
} // namespace rerun::blueprint::archetypes

namespace rerun {

    Result<Collection<ComponentBatch>>
        AsComponents<blueprint::archetypes::TensorViewFit>::as_batches(
            const blueprint::archetypes::TensorViewFit& archetype
        ) {
        using namespace blueprint::archetypes;
        std::vector<ComponentBatch> cells;
        cells.reserve(2);

        if (archetype.scaling.has_value()) {
            cells.push_back(archetype.scaling.value());
        }
        {
            auto result = ComponentBatch::from_indicator<TensorViewFit>();
            RR_RETURN_NOT_OK(result.error);
            cells.emplace_back(std::move(result.value));
        }

        return rerun::take_ownership(std::move(cells));
    }
} // namespace rerun
