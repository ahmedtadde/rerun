// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/rust/api.rs
// Based on "crates/store/re_types/definitions/rerun/testing/datatypes/fuzzy.fbs".

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

#[derive(Clone, Debug, PartialEq)]
pub enum AffixFuzzer3 {
    Degrees(f32),
    Craziness(Vec<crate::testing::datatypes::AffixFuzzer1>),
    FixedSizeShenanigans([f32; 3usize]),
    EmptyVariant,
}

::re_types_core::macros::impl_into_cow!(AffixFuzzer3);

impl ::re_types_core::Loggable for AffixFuzzer3 {
    #[inline]
    fn arrow_datatype() -> arrow::datatypes::DataType {
        #![allow(clippy::wildcard_imports)]
        use arrow::datatypes::*;
        DataType::Union(
            UnionFields::new(
                vec![0, 1, 2, 3, 4],
                vec![
                    Field::new("_null_markers", DataType::Null, true),
                    Field::new("degrees", DataType::Float32, false),
                    Field::new(
                        "craziness",
                        DataType::List(std::sync::Arc::new(Field::new(
                            "item",
                            <crate::testing::datatypes::AffixFuzzer1>::arrow_datatype(),
                            false,
                        ))),
                        false,
                    ),
                    Field::new(
                        "fixed_size_shenanigans",
                        DataType::FixedSizeList(
                            std::sync::Arc::new(Field::new("item", DataType::Float32, false)),
                            3,
                        ),
                        false,
                    ),
                    Field::new("empty_variant", DataType::Null, true),
                ],
            ),
            UnionMode::Dense,
        )
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
            // Dense Arrow union
            let data: Vec<_> = data
                .into_iter()
                .map(|datum| {
                    let datum: Option<::std::borrow::Cow<'a, Self>> = datum.map(Into::into);
                    datum
                })
                .collect();
            let field_type_ids = [0, 1, 2, 3, 4];
            let fields = vec![
                Field::new("_null_markers", DataType::Null, true),
                Field::new("degrees", DataType::Float32, false),
                Field::new(
                    "craziness",
                    DataType::List(std::sync::Arc::new(Field::new(
                        "item",
                        <crate::testing::datatypes::AffixFuzzer1>::arrow_datatype(),
                        false,
                    ))),
                    false,
                ),
                Field::new(
                    "fixed_size_shenanigans",
                    DataType::FixedSizeList(
                        std::sync::Arc::new(Field::new("item", DataType::Float32, false)),
                        3,
                    ),
                    false,
                ),
                Field::new("empty_variant", DataType::Null, true),
            ];
            let type_ids: Vec<i8> = data
                .iter()
                .map(|a| match a.as_deref() {
                    None => 0,
                    Some(Self::Degrees(_)) => 1i8,
                    Some(Self::Craziness(_)) => 2i8,
                    Some(Self::FixedSizeShenanigans(_)) => 3i8,
                    Some(Self::EmptyVariant) => 4i8,
                })
                .collect();
            let offsets = {
                let mut degrees_offset = 0;
                let mut craziness_offset = 0;
                let mut fixed_size_shenanigans_offset = 0;
                let mut empty_variant_offset = 0;
                let mut nulls_offset = 0;
                data.iter()
                    .map(|v| match v.as_deref() {
                        None => {
                            let offset = nulls_offset;
                            nulls_offset += 1;
                            offset
                        }
                        Some(Self::Degrees(_)) => {
                            let offset = degrees_offset;
                            degrees_offset += 1;
                            offset
                        }
                        Some(Self::Craziness(_)) => {
                            let offset = craziness_offset;
                            craziness_offset += 1;
                            offset
                        }
                        Some(Self::FixedSizeShenanigans(_)) => {
                            let offset = fixed_size_shenanigans_offset;
                            fixed_size_shenanigans_offset += 1;
                            offset
                        }
                        Some(Self::EmptyVariant) => {
                            let offset = empty_variant_offset;
                            empty_variant_offset += 1;
                            offset
                        }
                    })
                    .collect()
            };
            let children = vec![
                as_array_ref(NullArray::new(data.iter().filter(|v| v.is_none()).count())),
                {
                    let degrees: Vec<_> = data
                        .iter()
                        .filter_map(|datum| match datum.as_deref() {
                            Some(Self::Degrees(v)) => Some(v.clone()),
                            _ => None,
                        })
                        .collect();
                    let degrees_validity: Option<arrow::buffer::NullBuffer> = None;
                    as_array_ref(PrimitiveArray::<Float32Type>::new(
                        ScalarBuffer::from(degrees.into_iter().collect::<Vec<_>>()),
                        degrees_validity,
                    ))
                },
                {
                    let craziness: Vec<_> = data
                        .iter()
                        .filter_map(|datum| match datum.as_deref() {
                            Some(Self::Craziness(v)) => Some(v.clone()),
                            _ => None,
                        })
                        .collect();
                    let craziness_validity: Option<arrow::buffer::NullBuffer> = None;
                    {
                        let offsets = arrow::buffer::OffsetBuffer::<i32>::from_lengths(
                            craziness.iter().map(|datum| datum.len()),
                        );
                        let craziness_inner_data: Vec<_> =
                            craziness.into_iter().flatten().collect();
                        let craziness_inner_validity: Option<arrow::buffer::NullBuffer> = None;
                        as_array_ref(ListArray::try_new(
                            std::sync::Arc::new(Field::new(
                                "item",
                                <crate::testing::datatypes::AffixFuzzer1>::arrow_datatype(),
                                false,
                            )),
                            offsets,
                            {
                                _ = craziness_inner_validity;
                                crate::testing::datatypes::AffixFuzzer1::to_arrow_opt(
                                    craziness_inner_data.into_iter().map(Some),
                                )?
                            },
                            craziness_validity,
                        )?)
                    }
                },
                {
                    let fixed_size_shenanigans: Vec<_> = data
                        .iter()
                        .filter_map(|datum| match datum.as_deref() {
                            Some(Self::FixedSizeShenanigans(v)) => Some(v.clone()),
                            _ => None,
                        })
                        .collect();
                    let fixed_size_shenanigans_validity: Option<arrow::buffer::NullBuffer> = None;
                    {
                        let fixed_size_shenanigans_inner_data: Vec<_> =
                            fixed_size_shenanigans.into_iter().flatten().collect();
                        let fixed_size_shenanigans_inner_validity: Option<
                            arrow::buffer::NullBuffer,
                        > = None;
                        as_array_ref(FixedSizeListArray::new(
                            std::sync::Arc::new(Field::new("item", DataType::Float32, false)),
                            3,
                            as_array_ref(PrimitiveArray::<Float32Type>::new(
                                ScalarBuffer::from(
                                    fixed_size_shenanigans_inner_data
                                        .into_iter()
                                        .collect::<Vec<_>>(),
                                ),
                                fixed_size_shenanigans_inner_validity,
                            )),
                            fixed_size_shenanigans_validity,
                        ))
                    }
                },
                as_array_ref(NullArray::new(
                    data.iter()
                        .filter(|datum| matches!(datum.as_deref(), Some(Self::EmptyVariant)))
                        .count(),
                )),
            ];
            debug_assert_eq!(field_type_ids.len(), fields.len());
            debug_assert_eq!(fields.len(), children.len());
            as_array_ref(UnionArray::try_new(
                UnionFields::new(field_type_ids, fields),
                ScalarBuffer::from(type_ids),
                Some(offsets),
                children,
            )?)
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
                .downcast_ref::<arrow2::array::UnionArray>()
                .ok_or_else(|| {
                    let expected = Self::arrow_datatype();
                    let actual = arrow_data.data_type().clone();
                    DeserializationError::datatype_mismatch(expected, actual)
                })
                .with_context("rerun.testing.datatypes.AffixFuzzer3")?;
            if arrow_data.is_empty() {
                Vec::new()
            } else {
                let (arrow_data_types, arrow_data_arrays) =
                    (arrow_data.types(), arrow_data.fields());
                let arrow_data_offsets = arrow_data
                    .offsets()
                    .ok_or_else(|| {
                        let expected = Self::arrow_datatype();
                        let actual = arrow_data.data_type().clone();
                        DeserializationError::datatype_mismatch(expected, actual)
                    })
                    .with_context("rerun.testing.datatypes.AffixFuzzer3")?;
                if arrow_data_types.len() != arrow_data_offsets.len() {
                    return Err(DeserializationError::offset_slice_oob(
                        (0, arrow_data_types.len()),
                        arrow_data_offsets.len(),
                    ))
                    .with_context("rerun.testing.datatypes.AffixFuzzer3");
                }
                let degrees = {
                    if 1usize >= arrow_data_arrays.len() {
                        return Ok(Vec::new());
                    }
                    let arrow_data = &*arrow_data_arrays[1usize];
                    arrow_data
                        .as_any()
                        .downcast_ref::<Float32Array>()
                        .ok_or_else(|| {
                            let expected = DataType::Float32;
                            let actual = arrow_data.data_type().clone();
                            DeserializationError::datatype_mismatch(expected, actual)
                        })
                        .with_context("rerun.testing.datatypes.AffixFuzzer3#degrees")?
                        .into_iter()
                        .map(|opt| opt.copied())
                        .collect::<Vec<_>>()
                };
                let craziness = {
                    if 2usize >= arrow_data_arrays.len() {
                        return Ok(Vec::new());
                    }
                    let arrow_data = &*arrow_data_arrays[2usize];
                    {
                        let arrow_data = arrow_data
                            .as_any()
                            .downcast_ref::<arrow2::array::ListArray<i32>>()
                            .ok_or_else(|| {
                                let expected = DataType::List(std::sync::Arc::new(Field::new(
                                    "item",
                                    <crate::testing::datatypes::AffixFuzzer1>::arrow_datatype(),
                                    false,
                                )));
                                let actual = arrow_data.data_type().clone();
                                DeserializationError::datatype_mismatch(expected, actual)
                            })
                            .with_context("rerun.testing.datatypes.AffixFuzzer3#craziness")?;
                        if arrow_data.is_empty() {
                            Vec::new()
                        } else {
                            let arrow_data_inner = {
                                let arrow_data_inner = &**arrow_data.values();
                                crate::testing::datatypes::AffixFuzzer1::from_arrow2_opt(
                                    arrow_data_inner,
                                )
                                .with_context("rerun.testing.datatypes.AffixFuzzer3#craziness")?
                                .into_iter()
                                .collect::<Vec<_>>()
                            };
                            let offsets = arrow_data.offsets();
                            arrow2::bitmap::utils::ZipValidity::new_with_validity(
                                offsets.iter().zip(offsets.lengths()),
                                arrow_data.validity(),
                            )
                            .map(|elem| {
                                elem.map(|(start, len)| {
                                    let start = *start as usize;
                                    let end = start + len;
                                    if end > arrow_data_inner.len() {
                                        return Err(DeserializationError::offset_slice_oob(
                                            (start, end),
                                            arrow_data_inner.len(),
                                        ));
                                    }

                                    #[allow(unsafe_code, clippy::undocumented_unsafe_blocks)]
                                    let data =
                                        unsafe { arrow_data_inner.get_unchecked(start..end) };
                                    let data = data
                                        .iter()
                                        .cloned()
                                        .map(Option::unwrap_or_default)
                                        .collect();
                                    Ok(data)
                                })
                                .transpose()
                            })
                            .collect::<DeserializationResult<Vec<Option<_>>>>()?
                        }
                        .into_iter()
                    }
                    .collect::<Vec<_>>()
                };
                let fixed_size_shenanigans = {
                    if 3usize >= arrow_data_arrays.len() {
                        return Ok(Vec::new());
                    }
                    let arrow_data = &*arrow_data_arrays[3usize];
                    {
                        let arrow_data = arrow_data
                            .as_any()
                            .downcast_ref::<arrow2::array::FixedSizeListArray>()
                            .ok_or_else(|| {
                                let expected = DataType::FixedSizeList(
                                    std::sync::Arc::new(
                                        Field::new("item", DataType::Float32, false),
                                    ),
                                    3,
                                );
                                let actual = arrow_data.data_type().clone();
                                DeserializationError::datatype_mismatch(expected, actual)
                            })
                            .with_context(
                                "rerun.testing.datatypes.AffixFuzzer3#fixed_size_shenanigans",
                            )?;
                        if arrow_data.is_empty() {
                            Vec::new()
                        } else {
                            let offsets = (0..)
                                .step_by(3usize)
                                .zip((3usize..).step_by(3usize).take(arrow_data.len()));
                            let arrow_data_inner = {
                                let arrow_data_inner = &**arrow_data.values();
                                arrow_data_inner
                                    .as_any()
                                    .downcast_ref::<Float32Array>()
                                    .ok_or_else(|| {
                                        let expected = DataType::Float32;
                                        let actual = arrow_data_inner.data_type().clone();
                                        DeserializationError::datatype_mismatch(expected, actual)
                                    })
                                    .with_context(
                                        "rerun.testing.datatypes.AffixFuzzer3#fixed_size_shenanigans",
                                    )?
                                    .into_iter()
                                    .map(|opt| opt.copied())
                                    .collect::<Vec<_>>()
                            };
                            arrow2::bitmap::utils::ZipValidity::new_with_validity(
                                    offsets,
                                    arrow_data.validity(),
                                )
                                .map(|elem| {
                                    elem
                                        .map(|(start, end): (usize, usize)| {
                                            debug_assert!(end - start == 3usize);
                                            if end > arrow_data_inner.len() {
                                                return Err(
                                                    DeserializationError::offset_slice_oob(
                                                        (start, end),
                                                        arrow_data_inner.len(),
                                                    ),
                                                );
                                            }

                                            #[allow(unsafe_code, clippy::undocumented_unsafe_blocks)]
                                            let data = unsafe {
                                                arrow_data_inner.get_unchecked(start..end)
                                            };
                                            let data = data
                                                .iter()
                                                .cloned()
                                                .map(Option::unwrap_or_default);

                                            // NOTE: Unwrapping cannot fail: the length must be correct.
                                            #[allow(clippy::unwrap_used)]
                                            Ok(array_init::from_iter(data).unwrap())
                                        })
                                        .transpose()
                                })
                                .collect::<DeserializationResult<Vec<Option<_>>>>()?
                        }
                            .into_iter()
                    }
                        .collect::<Vec<_>>()
                };
                arrow_data_types
                    .iter()
                    .enumerate()
                    .map(|(i, typ)| {
                        let offset = arrow_data_offsets[i];
                        if *typ == 0 {
                            Ok(None)
                        } else {
                            Ok(
                                Some(
                                    match typ {
                                        1i8 => {
                                            Self::Degrees({
                                                if offset as usize >= degrees.len() {
                                                    return Err(
                                                            DeserializationError::offset_oob(offset as _, degrees.len()),
                                                        )
                                                        .with_context(
                                                            "rerun.testing.datatypes.AffixFuzzer3#degrees",
                                                        );
                                                }

                                                #[allow(unsafe_code, clippy::undocumented_unsafe_blocks)]
                                                unsafe { degrees.get_unchecked(offset as usize) }
                                                    .clone()
                                                    .ok_or_else(DeserializationError::missing_data)
                                                    .with_context(
                                                        "rerun.testing.datatypes.AffixFuzzer3#degrees",
                                                    )?
                                            })
                                        }
                                        2i8 => {
                                            Self::Craziness({
                                                if offset as usize >= craziness.len() {
                                                    return Err(
                                                            DeserializationError::offset_oob(
                                                                offset as _,
                                                                craziness.len(),
                                                            ),
                                                        )
                                                        .with_context(
                                                            "rerun.testing.datatypes.AffixFuzzer3#craziness",
                                                        );
                                                }

                                                #[allow(unsafe_code, clippy::undocumented_unsafe_blocks)]
                                                unsafe { craziness.get_unchecked(offset as usize) }
                                                    .clone()
                                                    .ok_or_else(DeserializationError::missing_data)
                                                    .with_context(
                                                        "rerun.testing.datatypes.AffixFuzzer3#craziness",
                                                    )?
                                            })
                                        }
                                        3i8 => {
                                            Self::FixedSizeShenanigans({
                                                if offset as usize >= fixed_size_shenanigans.len() {
                                                    return Err(
                                                            DeserializationError::offset_oob(
                                                                offset as _,
                                                                fixed_size_shenanigans.len(),
                                                            ),
                                                        )
                                                        .with_context(
                                                            "rerun.testing.datatypes.AffixFuzzer3#fixed_size_shenanigans",
                                                        );
                                                }

                                                #[allow(unsafe_code, clippy::undocumented_unsafe_blocks)]
                                                unsafe {
                                                    fixed_size_shenanigans.get_unchecked(offset as usize)
                                                }
                                                    .clone()
                                                    .ok_or_else(DeserializationError::missing_data)
                                                    .with_context(
                                                        "rerun.testing.datatypes.AffixFuzzer3#fixed_size_shenanigans",
                                                    )?
                                            })
                                        }
                                        4i8 => Self::EmptyVariant,
                                        _ => {
                                            return Err(
                                                DeserializationError::missing_union_arm(
                                                    Self::arrow_datatype(),
                                                    "<invalid>",
                                                    *typ as _,
                                                ),
                                            );
                                        }
                                    },
                                ),
                            )
                        }
                    })
                    .collect::<DeserializationResult<Vec<_>>>()
                    .with_context("rerun.testing.datatypes.AffixFuzzer3")?
            }
        })
    }
}

impl ::re_types_core::SizeBytes for AffixFuzzer3 {
    #[inline]
    fn heap_size_bytes(&self) -> u64 {
        #![allow(clippy::match_same_arms)]
        match self {
            Self::Degrees(v) => v.heap_size_bytes(),
            Self::Craziness(v) => v.heap_size_bytes(),
            Self::FixedSizeShenanigans(v) => v.heap_size_bytes(),
            Self::EmptyVariant => 0,
        }
    }

    #[inline]
    fn is_pod() -> bool {
        <f32>::is_pod()
            && <Vec<crate::testing::datatypes::AffixFuzzer1>>::is_pod()
            && <[f32; 3usize]>::is_pod()
    }
}
