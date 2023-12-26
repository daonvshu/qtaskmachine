#include "delaystate.h"

DelayState::DelayState(int delayMs, QState *parent)
    : LinearState(parent)
{
    LinearState::delayMs = delayMs;
}

TargetSignalTransition *DelayState::getSignalTransition() {
    return nullptr;
}

LinearState::TransitionType DelayState::getTransitionType() {
    return LinearState::IgnoreFeedbackWithDelay;
}