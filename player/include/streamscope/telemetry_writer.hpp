#pragma once

#include <chrono>
#include <fstream>
#include <string>

class TelemetryWriter
{
public:
    explicit TelemetryWriter(const std::string& filePath);

    void writeEvent(const std::string& jsonLine);

    long long timestampMs() const;

private:
    std::ofstream file_;
    std::chrono::steady_clock::time_point startTime_;
};
