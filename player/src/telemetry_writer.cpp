#include "streamscope/telemetry_writer.hpp"

#include <chrono>
#include <stdexcept>

TelemetryWriter::TelemetryWriter(
    const std::string& filePath
)
    : file_(filePath, std::ios::app),
      startTime_(std::chrono::steady_clock::now())
{
    if (!file_.is_open())
    {
        throw std::runtime_error(
            "Failed to open telemetry file: " + filePath
        );
    }
}

long long TelemetryWriter::timestampMs() const
{
    const auto now = std::chrono::steady_clock::now();

    const auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            now - startTime_
        );

    return elapsed.count();
}

void TelemetryWriter::writeEvent(
    const std::string& jsonLine
)
{
    file_ << jsonLine << '\n';
    file_.flush();
}
