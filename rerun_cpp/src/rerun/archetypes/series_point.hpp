// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/cpp/mod.rs
// Based on "crates/store/re_types/definitions/rerun/archetypes/series_point.fbs".

#pragma once

#include "../collection.hpp"
#include "../component_batch.hpp"
#include "../component_column.hpp"
#include "../components/color.hpp"
#include "../components/marker_shape.hpp"
#include "../components/marker_size.hpp"
#include "../components/name.hpp"
#include "../indicator_component.hpp"
#include "../result.hpp"

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace rerun::archetypes {
    /// **Archetype**: Define the style properties for a point series in a chart.
    ///
    /// This archetype only provides styling information and should be logged as static
    /// when possible. The underlying data needs to be logged to the same entity-path using
    /// `archetypes::Scalar`.
    ///
    /// ## Example
    ///
    /// ### Point series
    /// ![image](https://static.rerun.io/series_point_style/82207a705da6c086b28ce161db1db9e8b12258b7/full.png)
    ///
    /// ```cpp
    /// #include <rerun.hpp>
    ///
    /// #include <cmath>
    ///
    /// constexpr float TAU = 6.28318530717958647692528676655900577f;
    ///
    /// int main() {
    ///     const auto rec = rerun::RecordingStream("rerun_example_series_point_style");
    ///     rec.spawn().exit_on_failure();
    ///
    ///     // Set up plot styling:
    ///     // They are logged static as they don't change over time and apply to all timelines.
    ///     // Log two point series under a shared root so that they show in the same plot by default.
    ///     rec.log_static(
    ///         "trig/sin",
    ///         rerun::SeriesPoint()
    ///             .with_color({255, 0, 0})
    ///             .with_name("sin(0.01t)")
    ///             .with_marker(rerun::components::MarkerShape::Circle)
    ///             .with_marker_size(4)
    ///     );
    ///     rec.log_static(
    ///         "trig/cos",
    ///         rerun::SeriesPoint()
    ///             .with_color({0, 255, 0})
    ///             .with_name("cos(0.01t)")
    ///             .with_marker(rerun::components::MarkerShape::Cross)
    ///             .with_marker_size(2)
    ///     );
    ///
    ///     // Log the data on a timeline called "step".
    ///     for (int t = 0; t <static_cast<int>(TAU * 2.0 * 10.0); ++t) {
    ///         rec.set_time_sequence("step", t);
    ///
    ///         rec.log("trig/sin", rerun::Scalar(sin(static_cast<double>(t) / 10.0)));
    ///         rec.log("trig/cos", rerun::Scalar(cos(static_cast<double>(t) / 10.0f)));
    ///     }
    /// }
    /// ```
    struct SeriesPoint {
        /// Color for the corresponding series.
        std::optional<ComponentBatch> color;

        /// What shape to use to represent the point
        std::optional<ComponentBatch> marker;

        /// Display name of the series.
        ///
        /// Used in the legend.
        std::optional<ComponentBatch> name;

        /// Size of the marker.
        std::optional<ComponentBatch> marker_size;

      public:
        static constexpr const char IndicatorComponentName[] =
            "rerun.components.SeriesPointIndicator";

        /// Indicator component, used to identify the archetype when converting to a list of components.
        using IndicatorComponent = rerun::components::IndicatorComponent<IndicatorComponentName>;
        /// The name of the archetype as used in `ComponentDescriptor`s.
        static constexpr const char ArchetypeName[] = "rerun.archetypes.SeriesPoint";

        /// `ComponentDescriptor` for the `color` field.
        static constexpr auto Descriptor_color = ComponentDescriptor(
            ArchetypeName, "color", Loggable<rerun::components::Color>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `marker` field.
        static constexpr auto Descriptor_marker = ComponentDescriptor(
            ArchetypeName, "marker",
            Loggable<rerun::components::MarkerShape>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `name` field.
        static constexpr auto Descriptor_name = ComponentDescriptor(
            ArchetypeName, "name", Loggable<rerun::components::Name>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `marker_size` field.
        static constexpr auto Descriptor_marker_size = ComponentDescriptor(
            ArchetypeName, "marker_size",
            Loggable<rerun::components::MarkerSize>::Descriptor.component_name
        );

      public:
        SeriesPoint() = default;
        SeriesPoint(SeriesPoint&& other) = default;
        SeriesPoint(const SeriesPoint& other) = default;
        SeriesPoint& operator=(const SeriesPoint& other) = default;
        SeriesPoint& operator=(SeriesPoint&& other) = default;

        /// Update only some specific fields of a `SeriesPoint`.
        static SeriesPoint update_fields() {
            return SeriesPoint();
        }

        /// Clear all the fields of a `SeriesPoint`.
        static SeriesPoint clear_fields();

        /// Color for the corresponding series.
        SeriesPoint with_color(const rerun::components::Color& _color) && {
            color = ComponentBatch::from_loggable(_color, Descriptor_color).value_or_throw();
            return std::move(*this);
        }

        /// What shape to use to represent the point
        SeriesPoint with_marker(const rerun::components::MarkerShape& _marker) && {
            marker = ComponentBatch::from_loggable(_marker, Descriptor_marker).value_or_throw();
            return std::move(*this);
        }

        /// Display name of the series.
        ///
        /// Used in the legend.
        SeriesPoint with_name(const rerun::components::Name& _name) && {
            name = ComponentBatch::from_loggable(_name, Descriptor_name).value_or_throw();
            return std::move(*this);
        }

        /// Size of the marker.
        SeriesPoint with_marker_size(const rerun::components::MarkerSize& _marker_size) && {
            marker_size = ComponentBatch::from_loggable(_marker_size, Descriptor_marker_size)
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

} // namespace rerun::archetypes

namespace rerun {
    /// \private
    template <typename T>
    struct AsComponents;

    /// \private
    template <>
    struct AsComponents<archetypes::SeriesPoint> {
        /// Serialize all set component batches.
        static Result<std::vector<ComponentBatch>> serialize(
            const archetypes::SeriesPoint& archetype
        );
    };
} // namespace rerun
