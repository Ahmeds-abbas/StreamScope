#pragma once

enum class PlaybackState
{
    Idle,
    Buffering,
    Playing,
    Stalled,
    Ended,
    Error
};
