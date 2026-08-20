#include "streamscope/abr_selector.hpp"

const Representation* selectRepresentation(
    const std::vector<Representation>& representations,
    double throughputMbps,
    double safetyFactor
)
{
    if (representations.empty())
    {
        return nullptr;
    }

    const double safeThroughputBps =
        throughputMbps * 1'000'000.0 * safetyFactor;

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