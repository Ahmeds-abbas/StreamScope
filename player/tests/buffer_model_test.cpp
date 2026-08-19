#include <gtest/gtest.h>

#include "streamscope/buffer_model.hpp"

TEST(BufferModelTest, StartsEmpty)
{
    BufferModel buffer;

    EXPECT_DOUBLE_EQ(buffer.level(), 0.0);
    EXPECT_TRUE(buffer.empty());
}

TEST(BufferModelTest, AddsBufferedSeconds)
{
    BufferModel buffer;

    buffer.add(4.0);

    EXPECT_DOUBLE_EQ(buffer.level(), 4.0);
    EXPECT_FALSE(buffer.empty());
}

TEST(BufferModelTest, NeverBecomesNegative)
{
    BufferModel buffer;

    buffer.add(4.0);
    buffer.consume(10.0);

    EXPECT_DOUBLE_EQ(buffer.level(), 0.0);
    EXPECT_TRUE(buffer.empty());
}
