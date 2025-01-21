// This file is @generated by prost-build.
/// RerunChunk is arrow IPC encoded RecordBatch that has
/// rerun-specific semantic constraints and can be directly
/// converted to a Rerun Chunk (`re_chunk::Chunk`)
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct RerunChunk {
    /// encoder version used to encode the data
    #[prost(enumeration = "EncoderVersion", tag = "1")]
    pub encoder_version: i32,
    /// Data payload is Arrow IPC encoded RecordBatch
    #[prost(bytes = "vec", tag = "1000")]
    pub payload: ::prost::alloc::vec::Vec<u8>,
}
impl ::prost::Name for RerunChunk {
    const NAME: &'static str = "RerunChunk";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.RerunChunk".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.RerunChunk".into()
    }
}
/// unique recording identifier. At this point in time it is the same id as the ChunkStore's StoreId
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct RecordingId {
    #[prost(string, tag = "1")]
    pub id: ::prost::alloc::string::String,
}
impl ::prost::Name for RecordingId {
    const NAME: &'static str = "RecordingId";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.RecordingId".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.RecordingId".into()
    }
}
/// A recording can have multiple timelines, each is identified by a name, for example `log_tick`, `log_time`, etc.
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct Timeline {
    #[prost(string, tag = "1")]
    pub name: ::prost::alloc::string::String,
}
impl ::prost::Name for Timeline {
    const NAME: &'static str = "Timeline";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.Timeline".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.Timeline".into()
    }
}
/// A time range between start and end time points. Each 64 bit number can represent different time point data
/// depending on the timeline it is associated with. Time range is inclusive for both start and end time points.
#[derive(Clone, Copy, PartialEq, ::prost::Message)]
pub struct TimeRange {
    #[prost(int64, tag = "1")]
    pub start: i64,
    #[prost(int64, tag = "2")]
    pub end: i64,
}
impl ::prost::Name for TimeRange {
    const NAME: &'static str = "TimeRange";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.TimeRange".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.TimeRange".into()
    }
}
/// arrow IPC serialized schema
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct Schema {
    #[prost(bytes = "vec", tag = "1")]
    pub arrow_schema: ::prost::alloc::vec::Vec<u8>,
}
impl ::prost::Name for Schema {
    const NAME: &'static str = "Schema";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.Schema".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.Schema".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct Query {
    /// The subset of the database that the query will run on: a set of EntityPath(s) and their
    /// associated Component(s)
    #[prost(message, optional, tag = "1")]
    pub view_contents: ::core::option::Option<ViewContents>,
    /// Whether the view_contents should ignore semantically empty columns
    /// A semantically empty column is a column that either contains no data at all, or where all
    /// values are either nulls or empty arrays (\[\]).
    #[prost(bool, tag = "2")]
    pub include_semantically_empty_columns: bool,
    /// Whether the view_contents should ignore columns corresponding to indicator components
    /// Indicator components are marker components, generally automatically inserted by Rerun, that
    /// helps keep track of the original context in which a piece of data was logged/sent.
    #[prost(bool, tag = "3")]
    pub include_indicator_columns: bool,
    /// Whether the view_contents should ignore columns corresponding to Clear-related components
    #[prost(bool, tag = "4")]
    pub include_tombstone_columns: bool,
    /// The index used to filter out _rows_ from the view contents.
    /// Only rows where at least 1 column contains non-null data at that index will be kept in the
    /// final dataset. If left unspecified, the results will only contain static data.
    #[prost(message, optional, tag = "5")]
    pub filtered_index: ::core::option::Option<IndexColumnSelector>,
    /// The range of index values used to filter out _rows_ from the view contents
    /// Only rows where at least 1 of the view-contents contains non-null data within that range will be kept in
    /// the final dataset.
    /// This has no effect if filtered_index isn't set.
    /// This has no effect if using_index_values is set.
    #[prost(message, optional, tag = "6")]
    pub filtered_index_range: ::core::option::Option<IndexRange>,
    /// The specific index values used to filter out _rows_ from the view contents.
    /// Only rows where at least 1 column contains non-null data at these specific values will be kept
    /// in the final dataset.
    /// This has no effect if filtered_index isn't set.
    /// This has no effect if using_index_values is set.
    #[prost(message, optional, tag = "7")]
    pub filtered_index_values: ::core::option::Option<IndexValues>,
    /// The specific index values used to sample _rows_ from the view contents.
    /// The final dataset will contain one row per sampled index value, regardless of whether data
    /// existed for that index value in the view contents.
    /// The semantics of the query are consistent with all other settings: the results will be
    /// sorted on the filtered_index, and only contain unique index values.
    ///
    /// This has no effect if filtered_index isn't set.
    /// If set, this overrides both filtered_index_range and filtered_index_values.
    #[prost(message, optional, tag = "8")]
    pub using_index_values: ::core::option::Option<IndexValues>,
    /// The component column used to filter out _rows_ from the view contents.
    /// Only rows where this column contains non-null data be kept in the final dataset.
    #[prost(message, optional, tag = "9")]
    pub filtered_is_not_null: ::core::option::Option<ComponentColumnSelector>,
    /// The specific _columns_ to sample from the final view contents.
    /// The order of the samples will be respected in the final result.
    ///
    /// If unspecified, it means - everything.
    #[prost(message, optional, tag = "10")]
    pub column_selection: ::core::option::Option<ColumnSelection>,
    /// Specifies how null values should be filled in the returned dataframe.
    #[prost(enumeration = "SparseFillStrategy", tag = "11")]
    pub sparse_fill_strategy: i32,
}
impl ::prost::Name for Query {
    const NAME: &'static str = "Query";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.Query".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.Query".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct ColumnDescriptor {
    #[prost(oneof = "column_descriptor::DescriptorType", tags = "1, 2")]
    pub descriptor_type: ::core::option::Option<column_descriptor::DescriptorType>,
}
/// Nested message and enum types in `ColumnDescriptor`.
pub mod column_descriptor {
    #[derive(Clone, PartialEq, ::prost::Oneof)]
    pub enum DescriptorType {
        #[prost(message, tag = "1")]
        TimeColumn(super::TimeColumnDescriptor),
        #[prost(message, tag = "2")]
        ComponentColumn(super::ComponentColumnDescriptor),
    }
}
impl ::prost::Name for ColumnDescriptor {
    const NAME: &'static str = "ColumnDescriptor";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.ColumnDescriptor".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.ColumnDescriptor".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct TimeColumnDescriptor {
    /// The timeline this column is associated with.
    #[prost(message, optional, tag = "1")]
    pub timeline: ::core::option::Option<Timeline>,
    /// The Arrow datatype of the column.
    ///
    /// Currently this is a JSON-encoded `arrow-rs` `DataType`.
    #[prost(string, tag = "2")]
    pub datatype: ::prost::alloc::string::String,
}
impl ::prost::Name for TimeColumnDescriptor {
    const NAME: &'static str = "TimeColumnDescriptor";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.TimeColumnDescriptor".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.TimeColumnDescriptor".into()
    }
}
/// Describes a data/component column, such as `Position3D`.
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct ComponentColumnDescriptor {
    /// The path of the entity.
    #[prost(message, optional, tag = "1")]
    pub entity_path: ::core::option::Option<EntityPath>,
    /// Optional name of the `Archetype` associated with this data.
    #[prost(string, optional, tag = "2")]
    pub archetype_name: ::core::option::Option<::prost::alloc::string::String>,
    /// Optional name of the field within `Archetype` associated with this data.
    #[prost(string, optional, tag = "3")]
    pub archetype_field_name: ::core::option::Option<::prost::alloc::string::String>,
    /// Semantic name associated with this data.
    #[prost(string, tag = "4")]
    pub component_name: ::prost::alloc::string::String,
    /// The Arrow datatype of the column.
    ///
    /// Currently this is a JSON-encoded `arrow-rs` `DataType`.
    #[prost(string, tag = "5")]
    pub datatype: ::prost::alloc::string::String,
    /// Whether the column is a static column.
    #[prost(bool, tag = "6")]
    pub is_static: bool,
    /// Whether the column is a tombstone column.
    #[prost(bool, tag = "7")]
    pub is_tombstone: bool,
    /// Whether the column is an indicator column.
    #[prost(bool, tag = "8")]
    pub is_indicator: bool,
    /// Whether the column is semantically empty.
    #[prost(bool, tag = "9")]
    pub is_semantically_empty: bool,
}
impl ::prost::Name for ComponentColumnDescriptor {
    const NAME: &'static str = "ComponentColumnDescriptor";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.ComponentColumnDescriptor".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.ComponentColumnDescriptor".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct ColumnSelection {
    #[prost(message, repeated, tag = "1")]
    pub columns: ::prost::alloc::vec::Vec<ColumnSelector>,
}
impl ::prost::Name for ColumnSelection {
    const NAME: &'static str = "ColumnSelection";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.ColumnSelection".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.ColumnSelection".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct ColumnSelector {
    #[prost(oneof = "column_selector::SelectorType", tags = "2, 3")]
    pub selector_type: ::core::option::Option<column_selector::SelectorType>,
}
/// Nested message and enum types in `ColumnSelector`.
pub mod column_selector {
    #[derive(Clone, PartialEq, ::prost::Oneof)]
    pub enum SelectorType {
        #[prost(message, tag = "2")]
        ComponentColumn(super::ComponentColumnSelector),
        #[prost(message, tag = "3")]
        TimeColumn(super::TimeColumnSelector),
    }
}
impl ::prost::Name for ColumnSelector {
    const NAME: &'static str = "ColumnSelector";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.ColumnSelector".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.ColumnSelector".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct IndexColumnSelector {
    /// TODO(zehiko) we need to add support for other types of index selectors
    #[prost(message, optional, tag = "1")]
    pub timeline: ::core::option::Option<Timeline>,
}
impl ::prost::Name for IndexColumnSelector {
    const NAME: &'static str = "IndexColumnSelector";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.IndexColumnSelector".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.IndexColumnSelector".into()
    }
}
#[derive(Clone, Copy, PartialEq, ::prost::Message)]
pub struct IndexRange {
    /// TODO(zehiko) support for other ranges for other index selectors
    #[prost(message, optional, tag = "1")]
    pub time_range: ::core::option::Option<TimeRange>,
}
impl ::prost::Name for IndexRange {
    const NAME: &'static str = "IndexRange";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.IndexRange".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.IndexRange".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct IndexValues {
    /// TODO(zehiko) we need to add support for other types of index selectors
    #[prost(message, repeated, tag = "1")]
    pub time_points: ::prost::alloc::vec::Vec<TimeInt>,
}
impl ::prost::Name for IndexValues {
    const NAME: &'static str = "IndexValues";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.IndexValues".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.IndexValues".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct SampledIndexValues {
    #[prost(message, repeated, tag = "1")]
    pub sample_points: ::prost::alloc::vec::Vec<TimeInt>,
}
impl ::prost::Name for SampledIndexValues {
    const NAME: &'static str = "SampledIndexValues";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.SampledIndexValues".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.SampledIndexValues".into()
    }
}
/// A 64-bit number describing either nanoseconds, sequence numbers or fully static data.
#[derive(Clone, Copy, PartialEq, ::prost::Message)]
pub struct TimeInt {
    #[prost(int64, tag = "1")]
    pub time: i64,
}
impl ::prost::Name for TimeInt {
    const NAME: &'static str = "TimeInt";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.TimeInt".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.TimeInt".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct ViewContents {
    #[prost(message, repeated, tag = "1")]
    pub contents: ::prost::alloc::vec::Vec<ViewContentsPart>,
}
impl ::prost::Name for ViewContents {
    const NAME: &'static str = "ViewContents";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.ViewContents".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.ViewContents".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct ViewContentsPart {
    #[prost(message, optional, tag = "1")]
    pub path: ::core::option::Option<EntityPath>,
    #[prost(message, optional, tag = "2")]
    pub components: ::core::option::Option<ComponentsSet>,
}
impl ::prost::Name for ViewContentsPart {
    const NAME: &'static str = "ViewContentsPart";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.ViewContentsPart".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.ViewContentsPart".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct ComponentsSet {
    #[prost(message, repeated, tag = "1")]
    pub components: ::prost::alloc::vec::Vec<Component>,
}
impl ::prost::Name for ComponentsSet {
    const NAME: &'static str = "ComponentsSet";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.ComponentsSet".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.ComponentsSet".into()
    }
}
/// The unique identifier of an entity, e.g. `camera/3/points`
/// See <<https://www.rerun.io/docs/concepts/entity-path>> for more on entity paths.
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct EntityPath {
    #[prost(string, tag = "1")]
    pub path: ::prost::alloc::string::String,
}
impl ::prost::Name for EntityPath {
    const NAME: &'static str = "EntityPath";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.EntityPath".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.EntityPath".into()
    }
}
/// Component describes semantic data that can be used by any number of  rerun's archetypes.
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct Component {
    /// component name needs to be a string as user can define their own component
    #[prost(string, tag = "1")]
    pub name: ::prost::alloc::string::String,
}
impl ::prost::Name for Component {
    const NAME: &'static str = "Component";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.Component".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.Component".into()
    }
}
/// Used to telect a time column.
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct TimeColumnSelector {
    #[prost(message, optional, tag = "1")]
    pub timeline: ::core::option::Option<Timeline>,
}
impl ::prost::Name for TimeColumnSelector {
    const NAME: &'static str = "TimeColumnSelector";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.TimeColumnSelector".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.TimeColumnSelector".into()
    }
}
/// Used to select a component based on its EntityPath and Component name.
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct ComponentColumnSelector {
    #[prost(message, optional, tag = "1")]
    pub entity_path: ::core::option::Option<EntityPath>,
    #[prost(message, optional, tag = "2")]
    pub component: ::core::option::Option<Component>,
}
impl ::prost::Name for ComponentColumnSelector {
    const NAME: &'static str = "ComponentColumnSelector";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.ComponentColumnSelector".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.ComponentColumnSelector".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct ApplicationId {
    #[prost(string, tag = "1")]
    pub id: ::prost::alloc::string::String,
}
impl ::prost::Name for ApplicationId {
    const NAME: &'static str = "ApplicationId";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.ApplicationId".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.ApplicationId".into()
    }
}
#[derive(Clone, PartialEq, ::prost::Message)]
pub struct StoreId {
    #[prost(enumeration = "StoreKind", tag = "1")]
    pub kind: i32,
    #[prost(string, tag = "2")]
    pub id: ::prost::alloc::string::String,
}
impl ::prost::Name for StoreId {
    const NAME: &'static str = "StoreId";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.StoreId".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.StoreId".into()
    }
}
/// A date-time represented as nanoseconds since unix epoch
#[derive(Clone, Copy, PartialEq, ::prost::Message)]
pub struct Time {
    #[prost(int64, tag = "1")]
    pub nanos_since_epoch: i64,
}
impl ::prost::Name for Time {
    const NAME: &'static str = "Time";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.Time".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.Time".into()
    }
}
#[derive(Clone, Copy, PartialEq, ::prost::Message)]
pub struct Tuid {
    /// Approximate nanoseconds since epoch.
    #[prost(fixed64, tag = "1")]
    pub time_ns: u64,
    /// Initialized to something random on each thread,
    /// then incremented for each new `Tuid` being allocated.
    #[prost(fixed64, tag = "2")]
    pub inc: u64,
}
impl ::prost::Name for Tuid {
    const NAME: &'static str = "Tuid";
    const PACKAGE: &'static str = "rerun.common.v0";
    fn full_name() -> ::prost::alloc::string::String {
        "rerun.common.v0.Tuid".into()
    }
    fn type_url() -> ::prost::alloc::string::String {
        "/rerun.common.v0.Tuid".into()
    }
}
/// supported encoder versions for encoding data
/// See `RerunData` and `RerunChunkData` for its usage
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash, PartialOrd, Ord, ::prost::Enumeration)]
#[repr(i32)]
pub enum EncoderVersion {
    V0 = 0,
}
impl EncoderVersion {
    /// String value of the enum field names used in the ProtoBuf definition.
    ///
    /// The values are not transformed in any way and thus are considered stable
    /// (if the ProtoBuf definition does not change) and safe for programmatic use.
    pub fn as_str_name(&self) -> &'static str {
        match self {
            Self::V0 => "V0",
        }
    }
    /// Creates an enum from field names used in the ProtoBuf definition.
    pub fn from_str_name(value: &str) -> ::core::option::Option<Self> {
        match value {
            "V0" => Some(Self::V0),
            _ => None,
        }
    }
}
/// Specifies how null values should be filled in the returned dataframe.
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash, PartialOrd, Ord, ::prost::Enumeration)]
#[repr(i32)]
pub enum SparseFillStrategy {
    None = 0,
    LatestAtGlobal = 1,
}
impl SparseFillStrategy {
    /// String value of the enum field names used in the ProtoBuf definition.
    ///
    /// The values are not transformed in any way and thus are considered stable
    /// (if the ProtoBuf definition does not change) and safe for programmatic use.
    pub fn as_str_name(&self) -> &'static str {
        match self {
            Self::None => "NONE",
            Self::LatestAtGlobal => "LATEST_AT_GLOBAL",
        }
    }
    /// Creates an enum from field names used in the ProtoBuf definition.
    pub fn from_str_name(value: &str) -> ::core::option::Option<Self> {
        match value {
            "NONE" => Some(Self::None),
            "LATEST_AT_GLOBAL" => Some(Self::LatestAtGlobal),
            _ => None,
        }
    }
}
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash, PartialOrd, Ord, ::prost::Enumeration)]
#[repr(i32)]
pub enum StoreKind {
    Recording = 0,
    Blueprint = 1,
}
impl StoreKind {
    /// String value of the enum field names used in the ProtoBuf definition.
    ///
    /// The values are not transformed in any way and thus are considered stable
    /// (if the ProtoBuf definition does not change) and safe for programmatic use.
    pub fn as_str_name(&self) -> &'static str {
        match self {
            Self::Recording => "RECORDING",
            Self::Blueprint => "BLUEPRINT",
        }
    }
    /// Creates an enum from field names used in the ProtoBuf definition.
    pub fn from_str_name(value: &str) -> ::core::option::Option<Self> {
        match value {
            "RECORDING" => Some(Self::Recording),
            "BLUEPRINT" => Some(Self::Blueprint),
            _ => None,
        }
    }
}
