// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/cpp/mod.rs
// Based on "crates/store/re_types/definitions/rerun/archetypes/asset_video.fbs".

#pragma once

#include "../collection.hpp"
#include "../component_batch.hpp"
#include "../component_column.hpp"
#include "../components/blob.hpp"
#include "../components/media_type.hpp"
#include "../indicator_component.hpp"
#include "../result.hpp"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <utility>
#include <vector>

namespace rerun::archetypes {
    /// **Archetype**: A video binary.
    ///
    /// Only MP4 containers with AV1 are generally supported,
    /// though the web viewer supports more video codecs, depending on browser.
    ///
    /// See <https://rerun.io/docs/reference/video> for details of what is and isn't supported.
    ///
    /// In order to display a video, you also need to log a `archetypes::VideoFrameReference` for each frame.
    ///
    /// ## Examples
    ///
    /// ### Video with automatically determined frames
    /// ![image](https://static.rerun.io/video_manual_frames/320a44e1e06b8b3a3161ecbbeae3e04d1ccb9589/full.png)
    ///
    /// ```cpp
    /// #include <rerun.hpp>
    ///
    /// #include <iostream>
    ///
    /// using namespace std::chrono_literals;
    ///
    /// int main(int argc, char* argv[]) {
    ///     if (argc <2) {
    ///         // TODO(#7354): Only mp4 is supported for now.
    ///         std::cerr <<"Usage: " <<argv[0] <<" <path_to_video.[mp4]>" <<std::endl;
    ///         return 1;
    ///     }
    ///
    ///     const auto path = argv[1];
    ///
    ///     const auto rec = rerun::RecordingStream("rerun_example_asset_video_auto_frames");
    ///     rec.spawn().exit_on_failure();
    ///
    ///     // Log video asset which is referred to by frame references.
    ///     auto video_asset = rerun::AssetVideo::from_file(path).value_or_throw();
    ///     rec.log_static("video", video_asset);
    ///
    ///     // Send automatically determined video frame timestamps.
    ///     std::vector<std::chrono::nanoseconds> frame_timestamps_ns =
    ///         video_asset.read_frame_timestamps_ns().value_or_throw();
    ///     // Note timeline values don't have to be the same as the video timestamps.
    ///     auto time_column =
    ///         rerun::TimeColumn::from_durations("video_time", rerun::borrow(frame_timestamps_ns));
    ///
    ///     std::vector<rerun::components::VideoTimestamp> video_timestamps(frame_timestamps_ns.size());
    ///     for (size_t i = 0; i <frame_timestamps_ns.size(); i++) {
    ///         video_timestamps[i] = rerun::components::VideoTimestamp(frame_timestamps_ns[i]);
    ///     }
    ///     auto video_frame_reference_indicators =
    ///         rerun::ComponentColumn::from_indicators<rerun::VideoFrameReference>(
    ///             static_cast<uint32_t>(video_timestamps.size())
    ///         );
    ///
    ///     rec.send_columns(
    ///         "video",
    ///         time_column,
    ///         rerun::VideoFrameReference().with_many_timestamp(rerun::borrow(video_timestamps)).columns()
    ///     );
    /// }
    /// ```
    ///
    /// ### Demonstrates manual use of video frame references
    /// ![image](https://static.rerun.io/video_manual_frames/9f41c00f84a98cc3f26875fba7c1d2fa2bad7151/full.png)
    ///
    /// ```cpp
    /// #include <rerun.hpp>
    ///
    /// #include <iostream>
    ///
    /// using namespace std::chrono_literals;
    ///
    /// int main(int argc, char* argv[]) {
    ///     if (argc <2) {
    ///         // TODO(#7354): Only mp4 is supported for now.
    ///         std::cerr <<"Usage: " <<argv[0] <<" <path_to_video.[mp4]>" <<std::endl;
    ///         return 1;
    ///     }
    ///
    ///     const auto path = argv[1];
    ///
    ///     const auto rec = rerun::RecordingStream("rerun_example_asset_video_manual_frames");
    ///     rec.spawn().exit_on_failure();
    ///
    ///     // Log video asset which is referred to by frame references.
    ///     rec.log_static("video_asset", rerun::AssetVideo::from_file(path).value_or_throw());
    ///
    ///     // Create two entities, showing the same video frozen at different times.
    ///     rec.log("frame_1s", rerun::VideoFrameReference(1.0s).with_video_reference("video_asset"));
    ///     rec.log("frame_2s", rerun::VideoFrameReference(2.0s).with_video_reference("video_asset"));
    ///
    ///     // TODO(#5520): log blueprint once supported
    /// }
    /// ```
    struct AssetVideo {
        /// The asset's bytes.
        std::optional<ComponentBatch> blob;

        /// The Media Type of the asset.
        ///
        /// Supported values:
        /// * `video/mp4`
        ///
        /// If omitted, the viewer will try to guess from the data blob.
        /// If it cannot guess, it won't be able to render the asset.
        std::optional<ComponentBatch> media_type;

      public:
        static constexpr const char IndicatorComponentName[] =
            "rerun.components.AssetVideoIndicator";

        /// Indicator component, used to identify the archetype when converting to a list of components.
        using IndicatorComponent = rerun::components::IndicatorComponent<IndicatorComponentName>;
        /// The name of the archetype as used in `ComponentDescriptor`s.
        static constexpr const char ArchetypeName[] = "rerun.archetypes.AssetVideo";

        /// `ComponentDescriptor` for the `blob` field.
        static constexpr auto Descriptor_blob = ComponentDescriptor(
            ArchetypeName, "blob", Loggable<rerun::components::Blob>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `media_type` field.
        static constexpr auto Descriptor_media_type = ComponentDescriptor(
            ArchetypeName, "media_type",
            Loggable<rerun::components::MediaType>::Descriptor.component_name
        );

      public: // START of extensions from asset_video_ext.cpp:
        /// Creates a new `AssetVideo` from the file contents at `path`.
        ///
        /// The `MediaType` will be guessed from the file extension.
        ///
        /// If no `MediaType` can be guessed at the moment, the Rerun Viewer will try to guess one
        /// from the data at render-time. If it can't, rendering will fail with an error.
        static Result<AssetVideo> from_file(const std::filesystem::path& path);

        /// Creates a new `AssetVideo` from the given `bytes`.
        ///
        /// If no `MediaType` is specified, the Rerun Viewer will try to guess one from the data
        /// at render-time. If it can't, rendering will fail with an error.
        static AssetVideo from_bytes(
            rerun::Collection<uint8_t> bytes,
            std::optional<rerun::components::MediaType> media_type = {}
        ) {
            AssetVideo asset = AssetVideo(std::move(bytes));
            // TODO(jan): we could try and guess using magic bytes here, like rust does.
            if (media_type.has_value()) {
                return std::move(asset).with_media_type(media_type.value());
            }
            return asset;
        }

        /// Determines the presentation timestamps of all frames inside the video.
        ///
        /// Returned timestamps are in nanoseconds since start and are guaranteed to be monotonically increasing.
        Result<std::vector<std::chrono::nanoseconds>> read_frame_timestamps_ns() const;

        // END of extensions from asset_video_ext.cpp, start of generated code:

      public:
        AssetVideo() = default;
        AssetVideo(AssetVideo&& other) = default;
        AssetVideo(const AssetVideo& other) = default;
        AssetVideo& operator=(const AssetVideo& other) = default;
        AssetVideo& operator=(AssetVideo&& other) = default;

        explicit AssetVideo(rerun::components::Blob _blob)
            : blob(ComponentBatch::from_loggable(std::move(_blob), Descriptor_blob).value_or_throw()
              ) {}

        /// Update only some specific fields of a `AssetVideo`.
        static AssetVideo update_fields() {
            return AssetVideo();
        }

        /// Clear all the fields of a `AssetVideo`.
        static AssetVideo clear_fields();

        /// The asset's bytes.
        AssetVideo with_blob(const rerun::components::Blob& _blob) && {
            blob = ComponentBatch::from_loggable(_blob, Descriptor_blob).value_or_throw();
            return std::move(*this);
        }

        /// This method makes it possible to pack multiple `blob` in a single component batch.
        ///
        /// This only makes sense when used in conjunction with `columns`. `with_blob` should
        /// be used when logging a single row's worth of data.
        AssetVideo with_many_blob(const Collection<rerun::components::Blob>& _blob) && {
            blob = ComponentBatch::from_loggable(_blob, Descriptor_blob).value_or_throw();
            return std::move(*this);
        }

        /// The Media Type of the asset.
        ///
        /// Supported values:
        /// * `video/mp4`
        ///
        /// If omitted, the viewer will try to guess from the data blob.
        /// If it cannot guess, it won't be able to render the asset.
        AssetVideo with_media_type(const rerun::components::MediaType& _media_type) && {
            media_type =
                ComponentBatch::from_loggable(_media_type, Descriptor_media_type).value_or_throw();
            return std::move(*this);
        }

        /// This method makes it possible to pack multiple `media_type` in a single component batch.
        ///
        /// This only makes sense when used in conjunction with `columns`. `with_media_type` should
        /// be used when logging a single row's worth of data.
        AssetVideo with_many_media_type(const Collection<rerun::components::MediaType>& _media_type
        ) && {
            media_type =
                ComponentBatch::from_loggable(_media_type, Descriptor_media_type).value_or_throw();
            return std::move(*this);
        }

        /// Partitions the component data into multiple sub-batches.
        ///
        /// Specifically, this transforms the existing `ComponentBatch` data into `ComponentColumn`s
        /// instead, via `ComponentBatch::partitioned`.
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
    struct AsComponents<archetypes::AssetVideo> {
        /// Serialize all set component batches.
        static Result<Collection<ComponentBatch>> as_batches(const archetypes::AssetVideo& archetype
        );
    };
} // namespace rerun
