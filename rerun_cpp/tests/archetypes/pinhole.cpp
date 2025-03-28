#include "archetype_test.hpp"

#include <rerun/archetypes/pinhole.hpp>

using namespace rerun::archetypes;

#define TEST_TAG "[pinhole][archetypes]"

SCENARIO(
    "Pinhole archetype can be serialized with the same result for manually built instances and "
    "the builder pattern",
    TEST_TAG
) {
    GIVEN("Constructed from builder via from_half_sizes and manually") {
        // TODO(andreas): Allow this?
        // auto from_builder = Pinhole({{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f}}

        auto from_builder = Pinhole(rerun::datatypes::Mat3x3({
                                        {1.0f, 2.0f, 3.0f},
                                        {4.0f, 5.0f, 6.0f},
                                        {7.0f, 8.0f, 9.0f},
                                    }))
                                .with_resolution(1.0f, 2.0f);

        Pinhole from_manual;
        from_manual.image_from_camera =
            rerun::ComponentBatch::from_loggable(
                rerun::components::PinholeProjection(rerun::datatypes::Mat3x3({
                    {1.0f, 2.0f, 3.0f},
                    {4.0f, 5.0f, 6.0f},
                    {7.0f, 8.0f, 9.0f},
                })),
                Pinhole::Descriptor_image_from_camera
            )
                .value_or_throw();
        from_manual.resolution = rerun::ComponentBatch::from_loggable(
                                     rerun::components::Resolution(1.0f, 2.0f),
                                     Pinhole::Descriptor_resolution
        )
                                     .value_or_throw();

        test_compare_archetype_serialization(from_manual, from_builder);
    }

    GIVEN("Constructed from via from_focal_length_and_resolution and manually") {
        auto from_builder = Pinhole::from_focal_length_and_resolution({1.f, 2.f}, {3.f, 4.f});

        Pinhole from_manual;
        from_manual.image_from_camera =
            rerun::ComponentBatch::from_loggable(
                rerun::components::PinholeProjection(rerun::datatypes::Mat3x3({
                    {1.0f, 0.0f, 0.0f},
                    {0.0f, 2.0f, 0.0f},
                    {1.5f, 2.0f, 1.0f},
                })),
                Pinhole::Descriptor_image_from_camera
            )
                .value_or_throw();
        from_manual.resolution = rerun::ComponentBatch::from_loggable(
                                     rerun::components::Resolution(3.0f, 4.0f),
                                     Pinhole::Descriptor_resolution
        )
                                     .value_or_throw();

        test_compare_archetype_serialization(from_manual, from_builder);
    }
}
