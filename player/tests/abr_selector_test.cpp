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
        selectRepresentation(representations, 5.0);

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
        selectRepresentation(representations, 0.5);

    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->height, 360);
}

TEST(AbrSelectorTest, SelectsHighestRepresentationWhenBandwidthIsHigh)
{
    std::vector<Representation> representations = {
        {"360p/index.m3u8", 1'100'000, 604, 360},
        {"720p/index.m3u8", 3'750'000, 1206, 720},
        {"1080p/index.m3u8", 6'300'000, 1810, 1080}
    };

    const Representation* selected =
        selectRepresentation(representations, 20.0);

    ASSERT_NE(selected, nullptr);
    EXPECT_EQ(selected->height, 1080);
}

TEST(AbrSelectorTest, ReturnsNullForNoRepresentations)
{
    std::vector<Representation> representations;

    EXPECT_EQ(
        selectRepresentation(representations, 5.0),
        nullptr
    );
}