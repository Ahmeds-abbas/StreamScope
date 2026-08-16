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

class PlaybackStateMachine
{
public:
    PlaybackStateMachine();

    PlaybackState current() const;

    bool transitionTo(PlaybackState next);

private:
    PlaybackState currentState_;
};