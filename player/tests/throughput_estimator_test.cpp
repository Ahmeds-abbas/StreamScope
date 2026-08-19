#include <gtest/gtest.h>

#include "streamscope/throughput_estimator.hpp"

TEST(ThroughputEstimatorTest, CalculatesMbpsCorrectly)
{
    const double throughput =
        calculateThroughputMbps(750000, 1.5);

    EXPECT_DOUBLE_EQ(throughput, 4.0);
}

TEST(ThroughputEstimatorTest, ReturnsZeroForZeroDuration)
{
    EXPECT_DOUBLE_EQ(
        calculateThroughputMbps(750000, 0.0),
        0.0
    );
}

TEST(ThroughputEstimatorTest, ReturnsZeroForNegativeDuration)
{
    EXPECT_DOUBLE_EQ(
        calculateThroughputMbps(750000, -1.0),
        0.0
    );
}
