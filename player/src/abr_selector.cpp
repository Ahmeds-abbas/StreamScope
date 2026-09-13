#include "streamscope/abr_selector.hpp"

const Representation* selectRepresentation(
    const std::vector<Representation>& representations,
    double throughputMbps,
    double bufferSeconds,
    double safetyFactor
)
{
    if (representations.empty())
    {
        return nullptr;
    }

    double bufferFactor = 1.0;

    if (bufferSeconds < 8.0)
    {
        bufferFactor = 0.5;
    }
    else if (bufferSeconds < 20.0)
    {
        bufferFactor = 0.75;
    }

    const double safeThroughputBps =
        throughputMbps * 1'000'000.0 * safetyFactor * bufferFactor;

    const Representation* lowest = &representations.front();
    const Representation* selected = nullptr;

    for (const Representation& representation : representations)
    {
        if (representation.bandwidth < lowest->bandwidth)
        {
            lowest = &representation;
        }

        if (representation.bandwidth <= safeThroughputBps)
        {
            if (
                selected == nullptr ||
                representation.bandwidth > selected->bandwidth
            )
            {
                selected = &representation;
            }
        }
    }

    if (selected != nullptr)
    {
        return selected;
    }

    return lowest;
}
