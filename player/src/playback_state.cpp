#include "streamscope/playback_state.hpp"

PlaybackStateMachine::PlaybackStateMachine()
    : currentState_(PlaybackState::Idle)
{
}

PlaybackState PlaybackStateMachine::current() const
{
    return currentState_;
}

bool PlaybackStateMachine::transitionTo(PlaybackState next)
{
    switch (currentState_)
    {
        case PlaybackState::Idle:
            if (next == PlaybackState::Buffering)
            {
                currentState_ = next;
                return true;
            }
            break;

        case PlaybackState::Buffering:
            if (next == PlaybackState::Playing ||
                next == PlaybackState::Error)
            {
                currentState_ = next;
                return true;
            }
            break;

        case PlaybackState::Playing:
            if (next == PlaybackState::Stalled ||
                next == PlaybackState::Ended ||
                next == PlaybackState::Error)
            {
                currentState_ = next;
                return true;
            }
            break;

        case PlaybackState::Stalled:
            if (next == PlaybackState::Playing ||
                next == PlaybackState::Error)
            {
                currentState_ = next;
                return true;
            }
            break;

        case PlaybackState::Ended:
        case PlaybackState::Error:
            break;
    }

    return false;
}