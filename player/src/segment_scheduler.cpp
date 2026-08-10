#include "streamscope/segment_scheduler.hpp"

SegmentScheduler::SegmentScheduler(
    const std::vector<Segment>& segments
)
    : segments_(segments)
{
}

const Segment* SegmentScheduler::next()
{
    if (nextIndex_ >= segments_.size())
    {
        return nullptr;
    }

    const Segment* segment = &segments_[nextIndex_];

    nextIndex_++;

    return segment;
}