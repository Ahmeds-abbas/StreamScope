#include "streamscope/throughput_estimator.hpp"

double calculateThroughputMbps(
    std::size_t bytes,
    double durationSeconds
)
{
    if (durationSeconds <= 0.0)
    {
        return 0.0;
    }

    const double bits =
        static_cast<double>(bytes) * 8.0;

    return bits / durationSeconds / 1'000'000.0;
}
