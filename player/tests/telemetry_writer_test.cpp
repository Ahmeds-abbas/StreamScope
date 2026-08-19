#include <gtest/gtest.h>

#include "streamscope/telemetry_writer.hpp"

#include <chrono>
#include <cstdio>
#include <fstream>
#include <string>
#include <thread>

TEST(TelemetryWriterTest, TimestampsNeverGoBackwards)
{
    TelemetryWriter telemetry("/tmp/streamscope_timestamp_test.jsonl");

    const long long first = telemetry.timestampMs();

    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    const long long second = telemetry.timestampMs();

    EXPECT_GE(second, first);

    std::remove("/tmp/streamscope_timestamp_test.jsonl");
}

TEST(TelemetryWriterTest, WritesEventToFile)
{
    const std::string path = "/tmp/streamscope_writer_test.jsonl";

    {
        TelemetryWriter telemetry(path);
        telemetry.writeEvent("{\"event\":\"test\"}");
    }

    std::ifstream file(path);
    std::string line;

    std::getline(file, line);

    EXPECT_EQ(line, "{\"event\":\"test\"}");

    std::remove(path.c_str());
}
