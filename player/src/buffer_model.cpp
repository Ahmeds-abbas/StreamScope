#include "streamscope/buffer_model.hpp"

#include <algorithm>

void BufferModel::add(double seconds)
{
    if (seconds > 0.0)
    {
        bufferSeconds_ += seconds;
    }
}

void BufferModel::consume(double seconds)
{
    if (seconds <= 0.0)
    {
        return;
    }

    bufferSeconds_ = std::max(
        0.0,
        bufferSeconds_ - seconds
    );
}

double BufferModel::level() const
{
    return bufferSeconds_;
}

bool BufferModel::empty() const
{
    return bufferSeconds_ <= 0.0;
}