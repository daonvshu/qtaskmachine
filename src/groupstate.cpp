#include "groupstate.h"


GroupState::GroupState(QState *parent)
    : LinearState(parent)
{
}

TargetSignalTransition *GroupState::getSignalTransition() {
    return nullptr;
}

LinearState::TransitionType GroupState::getTransitionType() {
    return LinearState::WaitForFinished;
}
