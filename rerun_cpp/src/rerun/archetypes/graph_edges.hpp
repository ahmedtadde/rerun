// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/cpp/mod.rs
// Based on "crates/store/re_types/definitions/rerun/archetypes/graph_edges.fbs".

#pragma once

#include "../collection.hpp"
#include "../component_batch.hpp"
#include "../component_column.hpp"
#include "../components/graph_edge.hpp"
#include "../components/graph_type.hpp"
#include "../indicator_component.hpp"
#include "../result.hpp"

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace rerun::archetypes {
    /// **Archetype**: A list of edges in a graph.
    ///
    /// By default, edges are undirected.
    ///
    /// ## Example
    ///
    /// ### Simple directed graph
    /// ![image](https://static.rerun.io/graph_directed/ca29a37b65e1e0b6482251dce401982a0bc568fa/full.png)
    ///
    /// ```cpp
    /// #include <rerun.hpp>
    ///
    /// int main() {
    ///     const auto rec = rerun::RecordingStream("rerun_example_graph_directed");
    ///     rec.spawn().exit_on_failure();
    ///
    ///     rec.log(
    ///         "simple",
    ///         rerun::GraphNodes({"a", "b", "c"})
    ///             .with_positions({{0.0, 100.0}, {-100.0, 0.0}, {100.0, 0.0}})
    ///             .with_labels({"A", "B", "C"}),
    ///         rerun::GraphEdges({{"a", "b"}, {"b", "c"}, {"c", "a"}})
    ///             // Graphs are undirected by default.
    ///             .with_graph_type(rerun::components::GraphType::Directed)
    ///     );
    /// }
    /// ```
    struct GraphEdges {
        /// A list of node tuples.
        std::optional<ComponentBatch> edges;

        /// Specifies if the graph is directed or undirected.
        ///
        /// If no `components::GraphType` is provided, the graph is assumed to be undirected.
        std::optional<ComponentBatch> graph_type;

      public:
        static constexpr const char IndicatorComponentName[] =
            "rerun.components.GraphEdgesIndicator";

        /// Indicator component, used to identify the archetype when converting to a list of components.
        using IndicatorComponent = rerun::components::IndicatorComponent<IndicatorComponentName>;
        /// The name of the archetype as used in `ComponentDescriptor`s.
        static constexpr const char ArchetypeName[] = "rerun.archetypes.GraphEdges";

        /// `ComponentDescriptor` for the `edges` field.
        static constexpr auto Descriptor_edges = ComponentDescriptor(
            ArchetypeName, "edges",
            Loggable<rerun::components::GraphEdge>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `graph_type` field.
        static constexpr auto Descriptor_graph_type = ComponentDescriptor(
            ArchetypeName, "graph_type",
            Loggable<rerun::components::GraphType>::Descriptor.component_name
        );

      public:
        GraphEdges() = default;
        GraphEdges(GraphEdges&& other) = default;
        GraphEdges(const GraphEdges& other) = default;
        GraphEdges& operator=(const GraphEdges& other) = default;
        GraphEdges& operator=(GraphEdges&& other) = default;

        explicit GraphEdges(Collection<rerun::components::GraphEdge> _edges)
            : edges(ComponentBatch::from_loggable(std::move(_edges), Descriptor_edges)
                        .value_or_throw()) {}

        /// Update only some specific fields of a `GraphEdges`.
        static GraphEdges update_fields() {
            return GraphEdges();
        }

        /// Clear all the fields of a `GraphEdges`.
        static GraphEdges clear_fields();

        /// A list of node tuples.
        GraphEdges with_edges(const Collection<rerun::components::GraphEdge>& _edges) && {
            edges = ComponentBatch::from_loggable(_edges, Descriptor_edges).value_or_throw();
            return std::move(*this);
        }

        /// Specifies if the graph is directed or undirected.
        ///
        /// If no `components::GraphType` is provided, the graph is assumed to be undirected.
        GraphEdges with_graph_type(const rerun::components::GraphType& _graph_type) && {
            graph_type =
                ComponentBatch::from_loggable(_graph_type, Descriptor_graph_type).value_or_throw();
            return std::move(*this);
        }

        /// This method makes it possible to pack multiple `graph_type` in a single component batch.
        ///
        /// This only makes sense when used in conjunction with `columns`. `with_graph_type` should
        /// be used when logging a single row's worth of data.
        GraphEdges with_many_graph_type(const Collection<rerun::components::GraphType>& _graph_type
        ) && {
            graph_type =
                ComponentBatch::from_loggable(_graph_type, Descriptor_graph_type).value_or_throw();
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
    struct AsComponents<archetypes::GraphEdges> {
        /// Serialize all set component batches.
        static Result<Collection<ComponentBatch>> as_batches(const archetypes::GraphEdges& archetype
        );
    };
} // namespace rerun
