// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/cpp/mod.rs
// Based on "crates/store/re_types/definitions/rerun/blueprint/archetypes/map_zoom.fbs".

#pragma once

#include "../../blueprint/components/zoom_level.hpp"
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
    /// **Archetype**: Configuration of the map view zoom level.
    struct MapZoom {
        /// Zoom level for the map.
        ///
        /// Zoom level follow the [`OpenStreetMap` definition](https://wiki.openstreetmap.org/wiki/Zoom_levels).
        std::optional<ComponentBatch> zoom;

      public:
        static constexpr const char IndicatorComponentName[] =
            "rerun.blueprint.components.MapZoomIndicator";

        /// Indicator component, used to identify the archetype when converting to a list of components.
        using IndicatorComponent = rerun::components::IndicatorComponent<IndicatorComponentName>;
        /// The name of the archetype as used in `ComponentDescriptor`s.
        static constexpr const char ArchetypeName[] = "rerun.blueprint.archetypes.MapZoom";

        /// `ComponentDescriptor` for the `zoom` field.
        static constexpr auto Descriptor_zoom = ComponentDescriptor(
            ArchetypeName, "zoom",
            Loggable<rerun::blueprint::components::ZoomLevel>::Descriptor.component_name
        );

      public:
        MapZoom() = default;
        MapZoom(MapZoom&& other) = default;
        MapZoom(const MapZoom& other) = default;
        MapZoom& operator=(const MapZoom& other) = default;
        MapZoom& operator=(MapZoom&& other) = default;

        explicit MapZoom(rerun::blueprint::components::ZoomLevel _zoom)
            : zoom(ComponentBatch::from_loggable(std::move(_zoom), Descriptor_zoom).value_or_throw()
              ) {}

        /// Update only some specific fields of a `MapZoom`.
        static MapZoom update_fields() {
            return MapZoom();
        }

        /// Clear all the fields of a `MapZoom`.
        static MapZoom clear_fields();

        /// Zoom level for the map.
        ///
        /// Zoom level follow the [`OpenStreetMap` definition](https://wiki.openstreetmap.org/wiki/Zoom_levels).
        MapZoom with_zoom(const rerun::blueprint::components::ZoomLevel& _zoom) && {
            zoom = ComponentBatch::from_loggable(_zoom, Descriptor_zoom).value_or_throw();
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
    struct AsComponents<blueprint::archetypes::MapZoom> {
        /// Serialize all set component batches.
        static Result<Collection<ComponentBatch>> as_batches(
            const blueprint::archetypes::MapZoom& archetype
        );
    };
} // namespace rerun
