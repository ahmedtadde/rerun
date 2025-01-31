// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/cpp/mod.rs
// Based on "crates/store/re_types/definitions/rerun/blueprint/archetypes/scalar_axis.fbs".

#pragma once

#include "../../blueprint/components/lock_range_during_zoom.hpp"
#include "../../collection.hpp"
#include "../../component_batch.hpp"
#include "../../component_column.hpp"
#include "../../components/range1d.hpp"
#include "../../indicator_component.hpp"
#include "../../result.hpp"

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace rerun::blueprint::archetypes {
    /// **Archetype**: Configuration for the scalar axis of a plot.
    struct ScalarAxis {
        /// The range of the axis.
        ///
        /// If unset, the range well be automatically determined based on the queried data.
        std::optional<ComponentBatch> range;

        /// If enabled, the Y axis range will remain locked to the specified range when zooming.
        std::optional<ComponentBatch> zoom_lock;

      public:
        static constexpr const char IndicatorComponentName[] =
            "rerun.blueprint.components.ScalarAxisIndicator";

        /// Indicator component, used to identify the archetype when converting to a list of components.
        using IndicatorComponent = rerun::components::IndicatorComponent<IndicatorComponentName>;
        /// The name of the archetype as used in `ComponentDescriptor`s.
        static constexpr const char ArchetypeName[] = "rerun.blueprint.archetypes.ScalarAxis";

        /// `ComponentDescriptor` for the `range` field.
        static constexpr auto Descriptor_range = ComponentDescriptor(
            ArchetypeName, "range", Loggable<rerun::components::Range1D>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `zoom_lock` field.
        static constexpr auto Descriptor_zoom_lock = ComponentDescriptor(
            ArchetypeName, "zoom_lock",
            Loggable<rerun::blueprint::components::LockRangeDuringZoom>::Descriptor.component_name
        );

      public:
        ScalarAxis() = default;
        ScalarAxis(ScalarAxis&& other) = default;
        ScalarAxis(const ScalarAxis& other) = default;
        ScalarAxis& operator=(const ScalarAxis& other) = default;
        ScalarAxis& operator=(ScalarAxis&& other) = default;

        /// Update only some specific fields of a `ScalarAxis`.
        static ScalarAxis update_fields() {
            return ScalarAxis();
        }

        /// Clear all the fields of a `ScalarAxis`.
        static ScalarAxis clear_fields();

        /// The range of the axis.
        ///
        /// If unset, the range well be automatically determined based on the queried data.
        ScalarAxis with_range(const rerun::components::Range1D& _range) && {
            range = ComponentBatch::from_loggable(_range, Descriptor_range).value_or_throw();
            return std::move(*this);
        }

        /// If enabled, the Y axis range will remain locked to the specified range when zooming.
        ScalarAxis with_zoom_lock(
            const rerun::blueprint::components::LockRangeDuringZoom& _zoom_lock
        ) && {
            zoom_lock =
                ComponentBatch::from_loggable(_zoom_lock, Descriptor_zoom_lock).value_or_throw();
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
    struct AsComponents<blueprint::archetypes::ScalarAxis> {
        /// Serialize all set component batches.
        static Result<Collection<ComponentBatch>> as_batches(
            const blueprint::archetypes::ScalarAxis& archetype
        );
    };
} // namespace rerun
