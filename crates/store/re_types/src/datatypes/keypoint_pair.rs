// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/rust/api.rs
// Based on "crates/store/re_types/definitions/rerun/datatypes/keypoint_pair.fbs".

#![allow(unused_imports)]
#![allow(unused_parens)]
#![allow(clippy::clone_on_copy)]
#![allow(clippy::cloned_instead_of_copied)]
#![allow(clippy::map_flatten)]
#![allow(clippy::needless_question_mark)]
#![allow(clippy::new_without_default)]
#![allow(clippy::redundant_closure)]
#![allow(clippy::too_many_arguments)]
#![allow(clippy::too_many_lines)]

use ::re_types_core::external::arrow2;
use ::re_types_core::SerializationResult;
use ::re_types_core::{ComponentBatch, ComponentBatchCowWithDescriptor};
use ::re_types_core::{ComponentDescriptor, ComponentName};
use ::re_types_core::{DeserializationError, DeserializationResult};

/// **Datatype**: A connection between two [`datatypes::KeypointId`][crate::datatypes::KeypointId]s.
#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct KeypointPair {
    /// The first point of the pair.
    pub keypoint0: crate::datatypes::KeypointId,

    /// The second point of the pair.
    pub keypoint1: crate::datatypes::KeypointId,
}

::re_types_core::macros::impl_into_cow!(KeypointPair);

impl ::re_types_core::Loggable for KeypointPair {
    #[inline]
    fn arrow_datatype() -> arrow::datatypes::DataType {
        #![allow(clippy::wildcard_imports)]
        use arrow::datatypes::*;
        DataType::Struct(Fields::from(vec![
            Field::new(
                "keypoint0",
                <crate::datatypes::KeypointId>::arrow_datatype(),
                false,
            ),
            Field::new(
                "keypoint1",
                <crate::datatypes::KeypointId>::arrow_datatype(),
                false,
            ),
        ]))
    }

    fn to_arrow_opt<'a>(
        data: impl IntoIterator<Item = Option<impl Into<::std::borrow::Cow<'a, Self>>>>,
    ) -> SerializationResult<arrow::array::ArrayRef>
    where
        Self: Clone + 'a,
    {
        #![allow(clippy::wildcard_imports)]
        #![allow(clippy::manual_is_variant_and)]
        use ::re_types_core::{arrow_helpers::as_array_ref, Loggable as _, ResultExt as _};
        use arrow::{array::*, buffer::*, datatypes::*};
        Ok({
            let fields = Fields::from(vec![
                Field::new(
                    "keypoint0",
                    <crate::datatypes::KeypointId>::arrow_datatype(),
                    false,
                ),
                Field::new(
                    "keypoint1",
                    <crate::datatypes::KeypointId>::arrow_datatype(),
                    false,
                ),
            ]);
            let (somes, data): (Vec<_>, Vec<_>) = data
                .into_iter()
                .map(|datum| {
                    let datum: Option<::std::borrow::Cow<'a, Self>> = datum.map(Into::into);
                    (datum.is_some(), datum)
                })
                .unzip();
            let validity: Option<arrow::buffer::NullBuffer> = {
                let any_nones = somes.iter().any(|some| !*some);
                any_nones.then(|| somes.into())
            };
            as_array_ref(StructArray::new(
                fields,
                vec![
                    {
                        let (somes, keypoint0): (Vec<_>, Vec<_>) = data
                            .iter()
                            .map(|datum| {
                                let datum = datum.as_ref().map(|datum| datum.keypoint0.clone());
                                (datum.is_some(), datum)
                            })
                            .unzip();
                        let keypoint0_validity: Option<arrow::buffer::NullBuffer> = {
                            let any_nones = somes.iter().any(|some| !*some);
                            any_nones.then(|| somes.into())
                        };
                        as_array_ref(PrimitiveArray::<UInt16Type>::new(
                            ScalarBuffer::from(
                                keypoint0
                                    .into_iter()
                                    .map(|datum| datum.map(|datum| datum.0).unwrap_or_default())
                                    .collect::<Vec<_>>(),
                            ),
                            keypoint0_validity,
                        ))
                    },
                    {
                        let (somes, keypoint1): (Vec<_>, Vec<_>) = data
                            .iter()
                            .map(|datum| {
                                let datum = datum.as_ref().map(|datum| datum.keypoint1.clone());
                                (datum.is_some(), datum)
                            })
                            .unzip();
                        let keypoint1_validity: Option<arrow::buffer::NullBuffer> = {
                            let any_nones = somes.iter().any(|some| !*some);
                            any_nones.then(|| somes.into())
                        };
                        as_array_ref(PrimitiveArray::<UInt16Type>::new(
                            ScalarBuffer::from(
                                keypoint1
                                    .into_iter()
                                    .map(|datum| datum.map(|datum| datum.0).unwrap_or_default())
                                    .collect::<Vec<_>>(),
                            ),
                            keypoint1_validity,
                        ))
                    },
                ],
                validity,
            ))
        })
    }

    fn from_arrow2_opt(
        arrow_data: &dyn arrow2::array::Array,
    ) -> DeserializationResult<Vec<Option<Self>>>
    where
        Self: Sized,
    {
        #![allow(clippy::wildcard_imports)]
        use ::re_types_core::{Loggable as _, ResultExt as _};
        use arrow::datatypes::*;
        use arrow2::{array::*, buffer::*};
        Ok({
            let arrow_data = arrow_data
                .as_any()
                .downcast_ref::<arrow2::array::StructArray>()
                .ok_or_else(|| {
                    let expected = Self::arrow_datatype();
                    let actual = arrow_data.data_type().clone();
                    DeserializationError::datatype_mismatch(expected, actual)
                })
                .with_context("rerun.datatypes.KeypointPair")?;
            if arrow_data.is_empty() {
                Vec::new()
            } else {
                let (arrow_data_fields, arrow_data_arrays) =
                    (arrow_data.fields(), arrow_data.values());
                let arrays_by_name: ::std::collections::HashMap<_, _> = arrow_data_fields
                    .iter()
                    .map(|field| field.name.as_str())
                    .zip(arrow_data_arrays)
                    .collect();
                let keypoint0 = {
                    if !arrays_by_name.contains_key("keypoint0") {
                        return Err(DeserializationError::missing_struct_field(
                            Self::arrow_datatype(),
                            "keypoint0",
                        ))
                        .with_context("rerun.datatypes.KeypointPair");
                    }
                    let arrow_data = &**arrays_by_name["keypoint0"];
                    arrow_data
                        .as_any()
                        .downcast_ref::<UInt16Array>()
                        .ok_or_else(|| {
                            let expected = DataType::UInt16;
                            let actual = arrow_data.data_type().clone();
                            DeserializationError::datatype_mismatch(expected, actual)
                        })
                        .with_context("rerun.datatypes.KeypointPair#keypoint0")?
                        .into_iter()
                        .map(|opt| opt.copied())
                        .map(|res_or_opt| res_or_opt.map(crate::datatypes::KeypointId))
                };
                let keypoint1 = {
                    if !arrays_by_name.contains_key("keypoint1") {
                        return Err(DeserializationError::missing_struct_field(
                            Self::arrow_datatype(),
                            "keypoint1",
                        ))
                        .with_context("rerun.datatypes.KeypointPair");
                    }
                    let arrow_data = &**arrays_by_name["keypoint1"];
                    arrow_data
                        .as_any()
                        .downcast_ref::<UInt16Array>()
                        .ok_or_else(|| {
                            let expected = DataType::UInt16;
                            let actual = arrow_data.data_type().clone();
                            DeserializationError::datatype_mismatch(expected, actual)
                        })
                        .with_context("rerun.datatypes.KeypointPair#keypoint1")?
                        .into_iter()
                        .map(|opt| opt.copied())
                        .map(|res_or_opt| res_or_opt.map(crate::datatypes::KeypointId))
                };
                arrow2::bitmap::utils::ZipValidity::new_with_validity(
                    ::itertools::izip!(keypoint0, keypoint1),
                    arrow_data.validity(),
                )
                .map(|opt| {
                    opt.map(|(keypoint0, keypoint1)| {
                        Ok(Self {
                            keypoint0: keypoint0
                                .ok_or_else(DeserializationError::missing_data)
                                .with_context("rerun.datatypes.KeypointPair#keypoint0")?,
                            keypoint1: keypoint1
                                .ok_or_else(DeserializationError::missing_data)
                                .with_context("rerun.datatypes.KeypointPair#keypoint1")?,
                        })
                    })
                    .transpose()
                })
                .collect::<DeserializationResult<Vec<_>>>()
                .with_context("rerun.datatypes.KeypointPair")?
            }
        })
    }
}

impl ::re_types_core::SizeBytes for KeypointPair {
    #[inline]
    fn heap_size_bytes(&self) -> u64 {
        self.keypoint0.heap_size_bytes() + self.keypoint1.heap_size_bytes()
    }

    #[inline]
    fn is_pod() -> bool {
        <crate::datatypes::KeypointId>::is_pod() && <crate::datatypes::KeypointId>::is_pod()
    }
}
