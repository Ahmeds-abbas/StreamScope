#include <gtest/gtest.h>

#include "streamscope/playback_state.hpp"

TEST(PlaybackStateTest, StartsIdle)
{
    PlaybackStateMachine stateMachine;

    EXPECT_EQ(stateMachine.current(), PlaybackState::Idle);
}

TEST(PlaybackStateTest, AllowsValidTransitions)
{
    PlaybackStateMachine stateMachine;

    EXPECT_TRUE(stateMachine.transitionTo(PlaybackState::Buffering));
    EXPECT_TRUE(stateMachine.transitionTo(PlaybackState::Playing));
    EXPECT_TRUE(stateMachine.transitionTo(PlaybackState::Stalled));
    EXPECT_TRUE(stateMachine.transitionTo(PlaybackState::Playing));
    EXPECT_TRUE(stateMachine.transitionTo(PlaybackState::Ended));

    EXPECT_EQ(stateMachine.current(), PlaybackState::Ended);
}

TEST(PlaybackStateTest, RejectsInvalidTransition)
{
    PlaybackStateMachine stateMachine;

    EXPECT_FALSE(stateMachine.transitionTo(PlaybackState::Playing));
    EXPECT_EQ(stateMachine.current(), PlaybackState::Idle);
}

TEST(PlaybackStateTest, EndedIsTerminal)
{
    PlaybackStateMachine stateMachine;

    stateMachine.transitionTo(PlaybackState::Buffering);
    stateMachine.transitionTo(PlaybackState::Playing);
    stateMachine.transitionTo(PlaybackState::Ended);

    EXPECT_FALSE(stateMachine.transitionTo(PlaybackState::Playing));
    EXPECT_EQ(stateMachine.current(), PlaybackState::Ended);
}
