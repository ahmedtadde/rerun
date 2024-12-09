// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/rust/api.rs
// Based on "crates/store/re_types/definitions/rerun/testing/components/fuzzy.fbs".

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

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct AffixFuzzer9(pub ::re_types_core::ArrowString);

impl ::re_types_core::Component for AffixFuzzer9 {
    #[inline]
    fn descriptor() -> ComponentDescriptor {
        ComponentDescriptor::new("rerun.testing.components.AffixFuzzer9")
    }
}

::re_types_core::macros::impl_into_cow!(AffixFuzzer9);

impl ::re_types_core::Loggable for AffixFuzzer9 {
    #[inline]
    fn arrow_datatype() -> arrow::datatypes::DataType {
        #![allow(clippy::wildcard_imports)]
        use arrow::datatypes::*;
        DataType::Utf8
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
            let (somes, data0): (Vec<_>, Vec<_>) = data
                .into_iter()
                .map(|datum| {
                    let datum: Option<::std::borrow::Cow<'a, Self>> = datum.map(Into::into);
                    let datum = datum.map(|datum| datum.into_owned().0);
                    (datum.is_some(), datum)
                })
                .unzip();
            let data0_validity: Option<arrow::buffer::NullBuffer> = {
                let any_nones = somes.iter().any(|some| !*some);
                any_nones.then(|| somes.into())
            };
            {
                let offsets = arrow::buffer::OffsetBuffer::<i32>::from_lengths(
                    data0
                        .iter()
                        .map(|opt| opt.as_ref().map(|datum| datum.len()).unwrap_or_default()),
                );
                let inner_data: arrow::buffer::Buffer =
                    data0.into_iter().flatten().flat_map(|s| s.0).collect();

                #[allow(unsafe_code, clippy::undocumented_unsafe_blocks)]
                as_array_ref(unsafe {
                    StringArray::new_unchecked(offsets, inner_data, data0_validity)
                })
            }
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
                .downcast_ref::<arrow2::array::Utf8Array<i32>>()
                .ok_or_else(|| {
                    let expected = Self::arrow_datatype();
                    let actual = arrow_data.data_type().clone();
                    DeserializationError::datatype_mismatch(expected, actual)
                })
                .with_context("rerun.testing.components.AffixFuzzer9#single_string_required")?;
            let arrow_data_buf = arrow_data.values();
            let offsets = arrow_data.offsets();
            arrow2::bitmap::utils::ZipValidity::new_with_validity(
                offsets.iter().zip(offsets.lengths()),
                arrow_data.validity(),
            )
            .map(|elem| {
                elem.map(|(start, len)| {
                    let start = *start as usize;
                    let end = start + len;
                    if end > arrow_data_buf.len() {
                        return Err(DeserializationError::offset_slice_oob(
                            (start, end),
                            arrow_data_buf.len(),
                        ));
                    }

                    #[allow(unsafe_code, clippy::undocumented_unsafe_blocks)]
                    let data = unsafe { arrow_data_buf.clone().sliced_unchecked(start, len) };
                    Ok(data)
                })
                .transpose()
            })
            .map(|res_or_opt| {
                res_or_opt.map(|res_or_opt| res_or_opt.map(|v| ::re_types_core::ArrowString(v)))
            })
            .collect::<DeserializationResult<Vec<Option<_>>>>()
            .with_context("rerun.testing.components.AffixFuzzer9#single_string_required")?
            .into_iter()
        }
        .map(|v| v.ok_or_else(DeserializationError::missing_data))
        .map(|res| res.map(|v| Some(Self(v))))
        .collect::<DeserializationResult<Vec<Option<_>>>>()
        .with_context("rerun.testing.components.AffixFuzzer9#single_string_required")
        .with_context("rerun.testing.components.AffixFuzzer9")?)
    }
}

impl From<::re_types_core::ArrowString> for AffixFuzzer9 {
    #[inline]
    fn from(single_string_required: ::re_types_core::ArrowString) -> Self {
        Self(single_string_required)
    }
}

impl From<AffixFuzzer9> for ::re_types_core::ArrowString {
    #[inline]
    fn from(value: AffixFuzzer9) -> Self {
        value.0
    }
}

impl std::ops::Deref for AffixFuzzer9 {
    type Target = ::re_types_core::ArrowString;

    #[inline]
    fn deref(&self) -> &::re_types_core::ArrowString {
        &self.0
    }
}

impl std::ops::DerefMut for AffixFuzzer9 {
    #[inline]
    fn deref_mut(&mut self) -> &mut ::re_types_core::ArrowString {
        &mut self.0
    }
}

impl ::re_types_core::SizeBytes for AffixFuzzer9 {
    #[inline]
    fn heap_size_bytes(&self) -> u64 {
        self.0.heap_size_bytes()
    }

    #[inline]
    fn is_pod() -> bool {
        <::re_types_core::ArrowString>::is_pod()
    }
}
