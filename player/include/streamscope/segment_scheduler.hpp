#pragma once

#include "streamscope/hls_manifest.hpp"

class SegmentScheduler
{
public:
    explicit SegmentScheduler(
        const std::vector<Segment>& segments
    );

    const Segment* next();

private:
    const std::vector<Segment>& segments_;
    std::size_t nextIndex_ = 0;
};