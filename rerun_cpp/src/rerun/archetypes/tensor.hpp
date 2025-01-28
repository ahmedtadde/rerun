// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/cpp/mod.rs
// Based on "crates/store/re_types/definitions/rerun/archetypes/tensor.fbs".

#pragma once

#include "../collection.hpp"
#include "../component_batch.hpp"
#include "../component_column.hpp"
#include "../components/tensor_data.hpp"
#include "../components/value_range.hpp"
#include "../indicator_component.hpp"
#include "../result.hpp"

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace rerun::archetypes {
    /// **Archetype**: An N-dimensional array of numbers.
    ///
    /// Since the underlying `rerun::datatypes::TensorData` uses `rerun::Collection` internally,
    /// data can be passed in without a copy from raw pointers or by reference from `std::vector`/`std::array`/c-arrays.
    /// If needed, this "borrow-behavior" can be extended by defining your own `rerun::CollectionAdapter`.
    ///
    /// ## Example
    ///
    /// ### Simple tensor
    /// ![image](https://static.rerun.io/tensor_simple/baacb07712f7b706e3c80e696f70616c6c20b367/full.png)
    ///
    /// ```cpp
    /// #include <rerun.hpp>
    ///
    /// #include <algorithm> // std::generate
    /// #include <random>
    /// #include <vector>
    ///
    /// int main() {
    ///     const auto rec = rerun::RecordingStream("rerun_example_tensor");
    ///     rec.spawn().exit_on_failure();
    ///
    ///     std::default_random_engine gen;
    ///     // On MSVC uint8_t distributions are not supported.
    ///     std::uniform_int_distribution<int> dist(0, 255);
    ///
    ///     std::vector<uint8_t> data(8 * 6 * 3 * 5);
    ///     std::generate(data.begin(), data.end(), [&] { return static_cast<uint8_t>(dist(gen)); });
    ///
    ///     rec.log(
    ///         "tensor",
    ///         rerun::Tensor({8, 6, 3, 5}, data).with_dim_names({"width", "height", "channel", "batch"})
    ///     );
    /// }
    /// ```
    struct Tensor {
        /// The tensor data
        std::optional<ComponentBatch> data;

        /// The expected range of values.
        ///
        /// This is typically the expected range of valid values.
        /// Everything outside of the range is clamped to the range for the purpose of colormpaping.
        /// Any colormap applied for display, will map this range.
        ///
        /// If not specified, the range will be automatically estimated from the data.
        /// Note that the Viewer may try to guess a wider range than the minimum/maximum of values
        /// in the contents of the tensor.
        /// E.g. if all values are positive, some bigger than 1.0 and all smaller than 255.0,
        /// the Viewer will guess that the data likely came from an 8bit image, thus assuming a range of 0-255.
        std::optional<ComponentBatch> value_range;

      public:
        static constexpr const char IndicatorComponentName[] = "rerun.components.TensorIndicator";

        /// Indicator component, used to identify the archetype when converting to a list of components.
        using IndicatorComponent = rerun::components::IndicatorComponent<IndicatorComponentName>;
        /// The name of the archetype as used in `ComponentDescriptor`s.
        static constexpr const char ArchetypeName[] = "rerun.archetypes.Tensor";

        /// `ComponentDescriptor` for the `data` field.
        static constexpr auto Descriptor_data = ComponentDescriptor(
            ArchetypeName, "data",
            Loggable<rerun::components::TensorData>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `value_range` field.
        static constexpr auto Descriptor_value_range = ComponentDescriptor(
            ArchetypeName, "value_range",
            Loggable<rerun::components::ValueRange>::Descriptor.component_name
        );

      public: // START of extensions from tensor_ext.cpp:
        RR_DISABLE_MAYBE_UNINITIALIZED_PUSH

        /// New Tensor from dimensions and tensor buffer.
        Tensor(Collection<uint64_t> shape, datatypes::TensorBuffer buffer)
            : Tensor(datatypes::TensorData(std::move(shape), std::move(buffer))) {}

        RR_DISABLE_MAYBE_UNINITIALIZED_POP

        /// New tensor from dimensions and pointer to tensor data.
        ///
        /// Type must be one of the types supported by `rerun::datatypes::TensorData`.
        /// \param shape
        /// Shape of the image. Determines the number of elements expected to be in `data`.
        /// \param data_
        /// Target of the pointer must outlive the archetype.
        template <typename TElement>
        explicit Tensor(Collection<uint64_t> shape, const TElement* data_)
            : Tensor(datatypes::TensorData(std::move(shape), data_)) {}

        /// Update the `names` of the contained `TensorData` dimensions.
        ///
        /// Any existing Dimension names will be overwritten.
        ///
        /// If too many, or too few names are provided, this function will call
        /// Error::handle and then proceed to only update the subset of names that it can.
        Tensor with_dim_names(Collection<std::string> names) &&;

        // END of extensions from tensor_ext.cpp, start of generated code:

      public:
        Tensor() = default;
        Tensor(Tensor&& other) = default;
        Tensor(const Tensor& other) = default;
        Tensor& operator=(const Tensor& other) = default;
        Tensor& operator=(Tensor&& other) = default;

        explicit Tensor(rerun::components::TensorData _data)
            : data(ComponentBatch::from_loggable(std::move(_data), Descriptor_data).value_or_throw()
              ) {}

        /// Update only some specific fields of a `Tensor`.
        static Tensor update_fields() {
            return Tensor();
        }

        /// Clear all the fields of a `Tensor`.
        static Tensor clear_fields();

        /// The tensor data
        Tensor with_data(const rerun::components::TensorData& _data) && {
            data = ComponentBatch::from_loggable(_data, Descriptor_data).value_or_throw();
            return std::move(*this);
        }

        /// The expected range of values.
        ///
        /// This is typically the expected range of valid values.
        /// Everything outside of the range is clamped to the range for the purpose of colormpaping.
        /// Any colormap applied for display, will map this range.
        ///
        /// If not specified, the range will be automatically estimated from the data.
        /// Note that the Viewer may try to guess a wider range than the minimum/maximum of values
        /// in the contents of the tensor.
        /// E.g. if all values are positive, some bigger than 1.0 and all smaller than 255.0,
        /// the Viewer will guess that the data likely came from an 8bit image, thus assuming a range of 0-255.
        Tensor with_value_range(const rerun::components::ValueRange& _value_range) && {
            value_range = ComponentBatch::from_loggable(_value_range, Descriptor_value_range)
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
    struct AsComponents<archetypes::Tensor> {
        /// Serialize all set component batches.
        static Result<std::vector<ComponentBatch>> serialize(const archetypes::Tensor& archetype);
    };
} // namespace rerun
