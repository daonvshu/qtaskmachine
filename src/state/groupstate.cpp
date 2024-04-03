#include "groupstate.h"


GroupState::GroupState(QState *parent)
    : LinearState(parent)
{
}

void GroupState::onEntry(QEvent *event) {
    LinearState::onEntry(event);
    clearTransitions();
    addTransition(this, &QState::finished, getTargetState());
}