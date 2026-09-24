#include <gtest/gtest.h>

#include "streamscope/abr_selector.hpp"

#include <vector>

TEST(AbrSelectorTest, SelectsHighestSafeRepresentation)
{
    std::vector<Representation> representations = {
        {"360p/index.m3u8", 1'100'000, 604, 360},
        {"720p/index.m3u8", 3'750'000, 1206, 720},
        {"1080p/index.m3u8", 6'300'000, 1810, 1080}
    };

    const Representation* selected =
        selectRepresentation(representations, 5.0, 20.0);

    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->height, 720);
}

TEST(AbrSelectorTest, LowBufferSelectsConservativeRepresentation)
{
    std::vector<Representation> representations = {
        {"360p/index.m3u8", 1'100'000, 604, 360},
        {"720p/index.m3u8", 3'750'000, 1206, 720},
        {"1080p/index.m3u8", 6'300'000, 1810, 1080}
    };

    const Representation* selected =
        selectRepresentation(representations, 8.0, 4.0);

    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->height, 360);
}

TEST(AbrSelectorTest, MediumBufferSelectsModerateRepresentation)
{
    std::vector<Representation> representations = {
        {"360p/index.m3u8", 1'100'000, 604, 360},
        {"720p/index.m3u8", 3'750'000, 1206, 720},
        {"1080p/index.m3u8", 6'300'000, 1810, 1080}
    };

    const Representation* selected =
        selectRepresentation(representations, 8.0, 10.0);

    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->height, 720);
}

TEST(AbrSelectorTest, FallsBackToLowestRepresentation)
{
    std::vector<Representation> representations = {
        {"360p/index.m3u8", 1'100'000, 604, 360},
        {"720p/index.m3u8", 3'750'000, 1206, 720},
        {"1080p/index.m3u8", 6'300'000, 1810, 1080}
    };

    const Representation* selected =
        selectRepresentation(representations, 0.5, 0.0);

    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->height, 360);
}

TEST(AbrSelectorTest, HealthyBufferMatchesThroughputOnlySelection)
{
    std::vector<Representation> representations = {
        {"360p/index.m3u8", 1'100'000, 604, 360},
        {"720p/index.m3u8", 3'750'000, 1206, 720},
        {"1080p/index.m3u8", 6'300'000, 1810, 1080}
    };

    const Representation* selected =
        selectRepresentation(representations, 8.0, 20.0);

    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->height, 1080);
}

TEST(AbrSelectorTest, ReturnsNullForNoRepresentations)
{
    std::vector<Representation> representations;

    EXPECT_EQ(
        selectRepresentation(representations, 5.0, 0.0),
        nullptr
    );
}
