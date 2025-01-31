// DO NOT EDIT! This file was auto-generated by crates/build/re_types_builder/src/codegen/cpp/mod.rs
// Based on "crates/store/re_types/definitions/rerun/archetypes/transform3d.fbs".

#pragma once

#include "../collection.hpp"
#include "../compiler_utils.hpp"
#include "../component_batch.hpp"
#include "../component_column.hpp"
#include "../components/axis_length.hpp"
#include "../components/rotation_axis_angle.hpp"
#include "../components/rotation_quat.hpp"
#include "../components/scale3d.hpp"
#include "../components/transform_mat3x3.hpp"
#include "../components/transform_relation.hpp"
#include "../components/translation3d.hpp"
#include "../indicator_component.hpp"
#include "../rerun_sdk_export.hpp"
#include "../result.hpp"
#include "../rotation3d.hpp"

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace rerun::archetypes {
    /// **Archetype**: A transform between two 3D spaces, i.e. a pose.
    ///
    /// From the point of view of the entity's coordinate system,
    /// all components are applied in the inverse order they are listed here.
    /// E.g. if both a translation and a max3x3 transform are present,
    /// the 3x3 matrix is applied first, followed by the translation.
    ///
    /// Whenever you log this archetype, it will write all components, even if you do not explicitly set them.
    /// This means that if you first log a transform with only a translation, and then log one with only a rotation,
    /// it will be resolved to a transform with only a rotation.
    ///
    /// For transforms that affect only a single entity and do not propagate along the entity tree refer to `archetypes::InstancePoses3D`.
    ///
    /// ## Examples
    ///
    /// ### Variety of 3D transforms
    /// ![image](https://static.rerun.io/transform3d_simple/141368b07360ce3fcb1553079258ae3f42bdb9ac/full.png)
    ///
    /// ```cpp
    /// #include <rerun.hpp>
    ///
    /// constexpr float TAU = 6.28318530717958647692528676655900577f;
    ///
    /// int main() {
    ///     const auto rec = rerun::RecordingStream("rerun_example_transform3d");
    ///     rec.spawn().exit_on_failure();
    ///
    ///     auto arrow =
    ///         rerun::Arrows3D::from_vectors({{0.0f, 1.0f, 0.0f}}).with_origins({{0.0f, 0.0f, 0.0f}});
    ///
    ///     rec.log("base", arrow);
    ///
    ///     rec.log("base/translated", rerun::Transform3D::from_translation({1.0f, 0.0f, 0.0f}));
    ///     rec.log("base/translated", arrow);
    ///
    ///     rec.log(
    ///         "base/rotated_scaled",
    ///         rerun::Transform3D::from_rotation_scale(
    ///             rerun::RotationAxisAngle({0.0f, 0.0f, 1.0f}, rerun::Angle::radians(TAU / 8.0f)),
    ///             2.0f
    ///         )
    ///     );
    ///     rec.log("base/rotated_scaled", arrow);
    /// }
    /// ```
    ///
    /// ### Transform hierarchy
    /// ![image](https://static.rerun.io/transform_hierarchy/cb7be7a5a31fcb2efc02ba38e434849248f87554/full.png)
    ///
    /// ```cpp
    /// #include <rerun.hpp>
    ///
    /// constexpr float TAU = 6.28318530717958647692528676655900577f;
    ///
    /// int main() {
    ///     const auto rec = rerun::RecordingStream("rerun_example_transform3d_hierarchy");
    ///     rec.spawn().exit_on_failure();
    ///
    ///     // TODO(#5521): log two views as in the python example
    ///
    ///     rec.set_time_seconds("sim_time", 0.0);
    ///
    ///     // Planetary motion is typically in the XY plane.
    ///     rec.log_static("/", rerun::ViewCoordinates::RIGHT_HAND_Z_UP);
    ///
    ///     // Setup points, all are in the center of their own space:
    ///     rec.log(
    ///         "sun",
    ///         rerun::Points3D({{0.0f, 0.0f, 0.0f}})
    ///             .with_radii({1.0f})
    ///             .with_colors({rerun::Color(255, 200, 10)})
    ///     );
    ///     rec.log(
    ///         "sun/planet",
    ///         rerun::Points3D({{0.0f, 0.0f, 0.0f}})
    ///             .with_radii({0.4f})
    ///             .with_colors({rerun::Color(40, 80, 200)})
    ///     );
    ///     rec.log(
    ///         "sun/planet/moon",
    ///         rerun::Points3D({{0.0f, 0.0f, 0.0f}})
    ///             .with_radii({0.15f})
    ///             .with_colors({rerun::Color(180, 180, 180)})
    ///     );
    ///
    ///     // Draw fixed paths where the planet & moon move.
    ///     float d_planet = 6.0f;
    ///     float d_moon = 3.0f;
    ///     std::vector<std::array<float, 3>> planet_path, moon_path;
    ///     for (int i = 0; i <= 100; i++) {
    ///         float angle = static_cast<float>(i) * 0.01f * TAU;
    ///         float circle_x = std::sin(angle);
    ///         float circle_y = std::cos(angle);
    ///         planet_path.push_back({circle_x * d_planet, circle_y * d_planet, 0.0f});
    ///         moon_path.push_back({circle_x * d_moon, circle_y * d_moon, 0.0f});
    ///     }
    ///     rec.log("sun/planet_path", rerun::LineStrips3D(rerun::LineStrip3D(planet_path)));
    ///     rec.log("sun/planet/moon_path", rerun::LineStrips3D(rerun::LineStrip3D(moon_path)));
    ///
    ///     // Movement via transforms.
    ///     for (int i = 0; i <6 * 120; i++) {
    ///         float time = static_cast<float>(i) / 120.0f;
    ///         rec.set_time_seconds("sim_time", time);
    ///         float r_moon = time * 5.0f;
    ///         float r_planet = time * 2.0f;
    ///
    ///         rec.log(
    ///             "sun/planet",
    ///             rerun::Transform3D::from_translation_rotation(
    ///                 {std::sin(r_planet) * d_planet, std::cos(r_planet) * d_planet, 0.0f},
    ///                 rerun::RotationAxisAngle{
    ///                     {1.0, 0.0f, 0.0f},
    ///                     rerun::Angle::degrees(20.0f),
    ///                 }
    ///             )
    ///         );
    ///         rec.log(
    ///             "sun/planet/moon",
    ///             rerun::Transform3D::from_translation(
    ///                 {std::cos(r_moon) * d_moon, std::sin(r_moon) * d_moon, 0.0f}
    ///             )
    ///                 .with_relation(rerun::components::TransformRelation::ChildFromParent)
    ///         );
    ///     }
    /// }
    /// ```
    struct Transform3D {
        /// Translation vector.
        std::optional<ComponentBatch> translation;

        /// Rotation via axis + angle.
        std::optional<ComponentBatch> rotation_axis_angle;

        /// Rotation via quaternion.
        std::optional<ComponentBatch> quaternion;

        /// Scaling factor.
        std::optional<ComponentBatch> scale;

        /// 3x3 transformation matrix.
        std::optional<ComponentBatch> mat3x3;

        /// Specifies the relation this transform establishes between this entity and its parent.
        std::optional<ComponentBatch> relation;

        /// Visual length of the 3 axes.
        ///
        /// The length is interpreted in the local coordinate system of the transform.
        /// If the transform is scaled, the axes will be scaled accordingly.
        std::optional<ComponentBatch> axis_length;

      public:
        static constexpr const char IndicatorComponentName[] =
            "rerun.components.Transform3DIndicator";

        /// Indicator component, used to identify the archetype when converting to a list of components.
        using IndicatorComponent = rerun::components::IndicatorComponent<IndicatorComponentName>;
        /// The name of the archetype as used in `ComponentDescriptor`s.
        static constexpr const char ArchetypeName[] = "rerun.archetypes.Transform3D";

        /// `ComponentDescriptor` for the `translation` field.
        static constexpr auto Descriptor_translation = ComponentDescriptor(
            ArchetypeName, "translation",
            Loggable<rerun::components::Translation3D>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `rotation_axis_angle` field.
        static constexpr auto Descriptor_rotation_axis_angle = ComponentDescriptor(
            ArchetypeName, "rotation_axis_angle",
            Loggable<rerun::components::RotationAxisAngle>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `quaternion` field.
        static constexpr auto Descriptor_quaternion = ComponentDescriptor(
            ArchetypeName, "quaternion",
            Loggable<rerun::components::RotationQuat>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `scale` field.
        static constexpr auto Descriptor_scale = ComponentDescriptor(
            ArchetypeName, "scale", Loggable<rerun::components::Scale3D>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `mat3x3` field.
        static constexpr auto Descriptor_mat3x3 = ComponentDescriptor(
            ArchetypeName, "mat3x3",
            Loggable<rerun::components::TransformMat3x3>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `relation` field.
        static constexpr auto Descriptor_relation = ComponentDescriptor(
            ArchetypeName, "relation",
            Loggable<rerun::components::TransformRelation>::Descriptor.component_name
        );
        /// `ComponentDescriptor` for the `axis_length` field.
        static constexpr auto Descriptor_axis_length = ComponentDescriptor(
            ArchetypeName, "axis_length",
            Loggable<rerun::components::AxisLength>::Descriptor.component_name
        );

      public: // START of extensions from transform3d_ext.cpp:
        /// Identity transformation.
        ///
        /// Applying this transform does not alter an entity's transformation.
        RERUN_SDK_EXPORT static const Transform3D IDENTITY;

        /// Invalid transformation.
        ///
        /// Applying this transform will cause this entity and the entire subtree not to be visualized.
        RERUN_SDK_EXPORT static const Transform3D INVALID;

        /// Creates a new 3D transform from translation and matrix provided as 3 columns.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation_ \çopydoc Transform3D::translation
        /// \param columns Column vectors of 3x3 matrix.
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        ///
        /// _Implementation note:_ This overload is necessary, otherwise the array may be
        /// interpreted as bool and call the wrong overload.
        Transform3D(
            const components::Translation3D& translation_, const datatypes::Vec3D (&columns)[3],
            bool from_parent = false
        )
            : Transform3D(translation_, components::TransformMat3x3(columns), from_parent) {}

        /// Creates a new 3D transform from translation/matrix.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation_ \çopydoc Transform3D::translation
        /// \param mat3x3_ \copydoc Transform3D::mat3x3
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        Transform3D(
            const components::Translation3D& translation_,
            const components::TransformMat3x3& mat3x3_, bool from_parent = false
        ) {
            *this = Transform3D::clear_fields().with_translation(translation_).with_mat3x3(mat3x3_);
            if (from_parent) {
                *this =
                    std::move(*this).with_relation(components::TransformRelation::ChildFromParent);
            }
        }

        /// From a translation applied after a 3x3 matrix.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation \çopydoc Transform3D::translation
        /// \param mat3x3 \copydoc Transform3D::mat3x3
        static Transform3D from_translation_mat3x3(
            const components::Translation3D& translation, const components::TransformMat3x3& mat3x3
        ) {
            return Transform3D(translation, mat3x3, false);
        }

        /// From a translation applied after a 3x3 matrix provided as 3 columns.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation \çopydoc Transform3D::translation
        /// \param columns Column vectors of 3x3 matrix.
        static Transform3D from_translation_mat3x3(
            const components::Translation3D& translation, const datatypes::Vec3D (&columns)[3]
        ) {
            return Transform3D::from_translation_mat3x3(
                translation,
                components::TransformMat3x3(columns)
            );
        }

        /// From translation only.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation_ \çopydoc Transform3D::translation
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        Transform3D(const components::Translation3D& translation_, bool from_parent = false) {
            *this = Transform3D::clear_fields().with_translation(translation_);
            if (from_parent) {
                *this =
                    std::move(*this).with_relation(components::TransformRelation::ChildFromParent);
            }
        }

        /// From a translation.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation \çopydoc Transform3D::translation
        static Transform3D from_translation(const components::Translation3D& translation) {
            return Transform3D(translation, false);
        }

        /// From 3x3 matrix only.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param mat3x3_ \copydoc Transform3D::mat3x3
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        Transform3D(const components::TransformMat3x3& mat3x3_, bool from_parent = false) {
            *this = Transform3D::clear_fields().with_mat3x3(mat3x3_);
            if (from_parent) {
                *this =
                    std::move(*this).with_relation(components::TransformRelation::ChildFromParent);
            }
        }

        /// From 3x3 matrix only.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param mat3x3 \copydoc Transform3D::mat3x3
        static Transform3D from_mat3x3(const components::TransformMat3x3& mat3x3) {
            return Transform3D(mat3x3, false);
        }

        /// From 3x3 matrix provided as 3 columns only.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param columns Column vectors of 3x3 matrix.
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        Transform3D(const datatypes::Vec3D (&columns)[3], bool from_parent = false)
            : Transform3D(components::TransformMat3x3(columns), from_parent) {}

        /// From 3x3 matrix provided as 3 columns only.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param columns Column vectors of 3x3 matrix.
        static Transform3D from_mat3x3(const datatypes::Vec3D (&columns)[3]) {
            return Transform3D(components::TransformMat3x3(columns), false);
        }

        /// Creates a new 3D transform from translation/rotation/scale.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation_ \copydoc Transform3D::translation
        /// \param rotation Rotation represented either as a quaternion or axis + angle rotation.
        /// \param scale_ \copydoc Transform3D::scale
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        Transform3D(
            const components::Translation3D& translation_, const Rotation3D& rotation,
            const components::Scale3D& scale_, bool from_parent = false
        ) {
            *this = Transform3D::clear_fields()
                        .with_translation(translation_)
                        .with_scale(scale_)
                        .with_rotation(rotation);
            if (from_parent) {
                *this =
                    std::move(*this).with_relation(components::TransformRelation::ChildFromParent);
            }
        }

        /// Creates a new 3D transform from translation/rotation/uniform-scale.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation_ \copydoc Transform3D::translation
        /// \param rotation Rotation represented either as a quaternion or axis + angle rotation.
        /// \param uniform_scale Uniform scale factor that is applied to all axis equally.
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        ///
        /// _Implementation note:_ This explicit overload prevents interpretation of the float as
        /// bool, leading to a call to the wrong overload.
        Transform3D(
            const components::Translation3D& translation_, const Rotation3D& rotation,
            float uniform_scale, bool from_parent = false
        )
            : Transform3D(translation_, rotation, components::Scale3D(uniform_scale), from_parent) {
        }

        /// From a translation, applied after a rotation & scale, known as an affine transformation.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation \copydoc Transform3D::translation
        /// \param rotation Rotation represented either as a quaternion or axis + angle rotation.
        /// \param scale \copydoc Transform3D::scale
        static Transform3D from_translation_rotation_scale(
            const components::Translation3D& translation, const Rotation3D& rotation,
            const components::Scale3D& scale
        ) {
            return Transform3D(translation, rotation, scale, false);
        }

        /// From a translation, applied after a rotation & scale, known as an affine transformation.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation \copydoc Transform3D::translation
        /// \param rotation Rotation represented either as a quaternion or axis + angle rotation.
        /// \param uniform_scale Uniform scale factor that is applied to all axis equally.
        static Transform3D from_translation_rotation_scale(
            const components::Translation3D& translation, const Rotation3D& rotation,
            float uniform_scale
        ) {
            return Transform3D(translation, rotation, components::Scale3D(uniform_scale), false);
        }

        /// Creates a new rigid transform (translation & rotation only).
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation_ \copydoc Transform3D::translation
        /// \param rotation Rotation represented either as a quaternion or axis + angle rotation.
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        Transform3D(
            const components::Translation3D& translation_, const Rotation3D& rotation,
            bool from_parent = false
        ) {
            *this =
                Transform3D::clear_fields().with_translation(translation_).with_rotation(rotation);
            if (from_parent) {
                *this =
                    std::move(*this).with_relation(components::TransformRelation::ChildFromParent);
            }
        }

        /// From a rotation & scale.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation \copydoc Transform3D::translation
        /// \param rotation Rotation represented either as a quaternion or axis + angle rotation.
        static Transform3D from_translation_rotation(
            const components::Translation3D& translation, const Rotation3D& rotation
        ) {
            return Transform3D(translation, rotation, false);
        }

        /// From translation & scale only.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation_ \copydoc Transform3D::translation
        /// \param scale_ Transform3D::scale
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        Transform3D(
            const components::Translation3D& translation_, const components::Scale3D& scale_,
            bool from_parent = false
        ) {
            *this = Transform3D::clear_fields().with_translation(translation_).with_scale(scale_);
            if (from_parent) {
                *this =
                    std::move(*this).with_relation(components::TransformRelation::ChildFromParent);
            }
        }

        /// From a translation applied after a scale.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation \copydoc Transform3D::translation
        /// \param scale Transform3D::scale
        static Transform3D from_translation_scale(
            const components::Translation3D& translation, const components::Scale3D& scale
        ) {
            return Transform3D(translation, scale, false);
        }

        /// From translation & uniform scale only.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param translation_ \copydoc Transform3D::translation
        /// \param uniform_scale Uniform scale factor that is applied to all axis equally.
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        ///
        /// _Implementation note:_ This explicit overload prevents interpretation of the float as
        /// bool, leading to a call to the wrong overload.
        Transform3D(
            const components::Translation3D& translation_, float uniform_scale,
            bool from_parent = false
        )
            : Transform3D(translation_, components::Scale3D(uniform_scale), from_parent) {}

        /// From rotation & scale.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param rotation Rotation represented either as a quaternion or axis + angle rotation.
        /// \param scale_ Transform3D::scale
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        Transform3D(
            const Rotation3D& rotation, const components::Scale3D& scale_, bool from_parent = false
        ) {
            *this = Transform3D::clear_fields().with_scale(scale_).with_rotation(rotation);
            if (from_parent) {
                *this =
                    std::move(*this).with_relation(components::TransformRelation::ChildFromParent);
            }
        }

        /// From rotation & uniform scale.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param rotation Rotation represented either as a quaternion or axis + angle rotation.
        /// \param uniform_scale Uniform scale factor that is applied to all axis equally.
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        ///
        /// _Implementation note:_ This explicit overload prevents interpretation of the float as
        /// bool, leading to a call to the wrong overload.
        Transform3D(const Rotation3D& rotation, float uniform_scale, bool from_parent = false)
            : Transform3D(rotation, components::Scale3D(uniform_scale), from_parent) {}

        /// From a rotation & scale.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param rotation Rotation represented either as a quaternion or axis + angle rotation.
        /// \param scale Transform3D::scale
        static Transform3D from_rotation_scale(
            const Rotation3D& rotation, const components::Scale3D& scale
        ) {
            return Transform3D(rotation, scale, false);
        }

        /// From a rotation & uniform scale.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param rotation Rotation represented either as a quaternion or axis + angle rotation.
        /// \param uniform_scale Uniform scale factor that is applied to all axis equally.
        static Transform3D from_rotation_scale(const Rotation3D& rotation, float uniform_scale) {
            return Transform3D(rotation, components::Scale3D(uniform_scale), false);
        }

        /// From rotation only.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param rotation Rotation represented either as a quaternion or axis + angle rotation.
        /// \param from_parent If true, the transform relation to `TransformRelation::ChildFromParent`.
        Transform3D(const Rotation3D& rotation, bool from_parent = false) {
            *this = Transform3D::clear_fields().with_rotation(rotation);
            if (from_parent) {
                *this =
                    std::move(*this).with_relation(components::TransformRelation::ChildFromParent);
            }
        }

        /// From rotation only.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param rotation Rotation represented either as a quaternion or axis + angle rotation.
        static Transform3D from_rotation(const Rotation3D& rotation) {
            return Transform3D(rotation, false);
        }

        /// From scale only.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param scale_ If true, the transform relation to `TransformRelation::ChildFromParent`.
        /// \param from_parent \copydoc Transform3D::scale
        Transform3D(const components::Scale3D& scale_, bool from_parent = false) {
            *this = Transform3D::clear_fields().with_scale(scale_);
            if (from_parent) {
                *this =
                    std::move(*this).with_relation(components::TransformRelation::ChildFromParent);
            }
        }

        /// From scale only.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param scale Transform3D::scale
        static Transform3D from_scale(const components::Scale3D& scale) {
            return Transform3D(scale, false);
        }

        /// From scale only.
        /// Clears out all other components like `Transform3D::clear_fields`.
        ///
        /// \param uniform_scale Uniform scale factor that is applied to all axis equally.
        static Transform3D from_scale(float uniform_scale) {
            return Transform3D(components::Scale3D(uniform_scale), false);
        }

        /// Set the rotation component of the transform using the `rerun::Rotation3D` utility.
        Transform3D with_rotation(const Rotation3D& rotation) && {
            if (rotation.axis_angle.has_value()) {
                *this = std::move(*this).with_rotation_axis_angle(rotation.axis_angle.value());
            }
            if (rotation.quaternion.has_value()) {
                *this = std::move(*this).with_quaternion(rotation.quaternion.value());
            }
            return std::move(*this);
        }

        // END of extensions from transform3d_ext.cpp, start of generated code:

      public:
        Transform3D() = default;
        Transform3D(Transform3D&& other) = default;
        Transform3D(const Transform3D& other) = default;
        Transform3D& operator=(const Transform3D& other) = default;
        Transform3D& operator=(Transform3D&& other) = default;

        /// Update only some specific fields of a `Transform3D`.
        static Transform3D update_fields() {
            return Transform3D();
        }

        /// Clear all the fields of a `Transform3D`.
        static Transform3D clear_fields();

        /// Translation vector.
        Transform3D with_translation(const rerun::components::Translation3D& _translation) && {
            translation = ComponentBatch::from_loggable(_translation, Descriptor_translation)
                              .value_or_throw();
            return std::move(*this);
        }

        /// This method makes it possible to pack multiple `translation` in a single component batch.
        ///
        /// This only makes sense when used in conjunction with `columns`. `with_translation` should
        /// be used when logging a single row's worth of data.
        Transform3D with_many_translation(
            const Collection<rerun::components::Translation3D>& _translation
        ) && {
            translation = ComponentBatch::from_loggable(_translation, Descriptor_translation)
                              .value_or_throw();
            return std::move(*this);
        }

        /// Rotation via axis + angle.
        Transform3D with_rotation_axis_angle(
            const rerun::components::RotationAxisAngle& _rotation_axis_angle
        ) && {
            rotation_axis_angle =
                ComponentBatch::from_loggable(_rotation_axis_angle, Descriptor_rotation_axis_angle)
                    .value_or_throw();
            return std::move(*this);
        }

        /// This method makes it possible to pack multiple `rotation_axis_angle` in a single component batch.
        ///
        /// This only makes sense when used in conjunction with `columns`. `with_rotation_axis_angle` should
        /// be used when logging a single row's worth of data.
        Transform3D with_many_rotation_axis_angle(
            const Collection<rerun::components::RotationAxisAngle>& _rotation_axis_angle
        ) && {
            rotation_axis_angle =
                ComponentBatch::from_loggable(_rotation_axis_angle, Descriptor_rotation_axis_angle)
                    .value_or_throw();
            return std::move(*this);
        }

        /// Rotation via quaternion.
        Transform3D with_quaternion(const rerun::components::RotationQuat& _quaternion) && {
            quaternion =
                ComponentBatch::from_loggable(_quaternion, Descriptor_quaternion).value_or_throw();
            return std::move(*this);
        }

        /// This method makes it possible to pack multiple `quaternion` in a single component batch.
        ///
        /// This only makes sense when used in conjunction with `columns`. `with_quaternion` should
        /// be used when logging a single row's worth of data.
        Transform3D with_many_quaternion(
            const Collection<rerun::components::RotationQuat>& _quaternion
        ) && {
            quaternion =
                ComponentBatch::from_loggable(_quaternion, Descriptor_quaternion).value_or_throw();
            return std::move(*this);
        }

        /// Scaling factor.
        Transform3D with_scale(const rerun::components::Scale3D& _scale) && {
            scale = ComponentBatch::from_loggable(_scale, Descriptor_scale).value_or_throw();
            return std::move(*this);
        }

        /// This method makes it possible to pack multiple `scale` in a single component batch.
        ///
        /// This only makes sense when used in conjunction with `columns`. `with_scale` should
        /// be used when logging a single row's worth of data.
        Transform3D with_many_scale(const Collection<rerun::components::Scale3D>& _scale) && {
            scale = ComponentBatch::from_loggable(_scale, Descriptor_scale).value_or_throw();
            return std::move(*this);
        }

        /// 3x3 transformation matrix.
        Transform3D with_mat3x3(const rerun::components::TransformMat3x3& _mat3x3) && {
            mat3x3 = ComponentBatch::from_loggable(_mat3x3, Descriptor_mat3x3).value_or_throw();
            return std::move(*this);
        }

        /// This method makes it possible to pack multiple `mat3x3` in a single component batch.
        ///
        /// This only makes sense when used in conjunction with `columns`. `with_mat3x3` should
        /// be used when logging a single row's worth of data.
        Transform3D with_many_mat3x3(const Collection<rerun::components::TransformMat3x3>& _mat3x3
        ) && {
            mat3x3 = ComponentBatch::from_loggable(_mat3x3, Descriptor_mat3x3).value_or_throw();
            return std::move(*this);
        }

        /// Specifies the relation this transform establishes between this entity and its parent.
        Transform3D with_relation(const rerun::components::TransformRelation& _relation) && {
            relation =
                ComponentBatch::from_loggable(_relation, Descriptor_relation).value_or_throw();
            return std::move(*this);
        }

        /// This method makes it possible to pack multiple `relation` in a single component batch.
        ///
        /// This only makes sense when used in conjunction with `columns`. `with_relation` should
        /// be used when logging a single row's worth of data.
        Transform3D with_many_relation(
            const Collection<rerun::components::TransformRelation>& _relation
        ) && {
            relation =
                ComponentBatch::from_loggable(_relation, Descriptor_relation).value_or_throw();
            return std::move(*this);
        }

        /// Visual length of the 3 axes.
        ///
        /// The length is interpreted in the local coordinate system of the transform.
        /// If the transform is scaled, the axes will be scaled accordingly.
        Transform3D with_axis_length(const rerun::components::AxisLength& _axis_length) && {
            axis_length = ComponentBatch::from_loggable(_axis_length, Descriptor_axis_length)
                              .value_or_throw();
            return std::move(*this);
        }

        /// This method makes it possible to pack multiple `axis_length` in a single component batch.
        ///
        /// This only makes sense when used in conjunction with `columns`. `with_axis_length` should
        /// be used when logging a single row's worth of data.
        Transform3D with_many_axis_length(
            const Collection<rerun::components::AxisLength>& _axis_length
        ) && {
            axis_length = ComponentBatch::from_loggable(_axis_length, Descriptor_axis_length)
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
    struct AsComponents<archetypes::Transform3D> {
        /// Serialize all set component batches.
        static Result<Collection<ComponentBatch>> as_batches(
            const archetypes::Transform3D& archetype
        );
    };
} // namespace rerun
