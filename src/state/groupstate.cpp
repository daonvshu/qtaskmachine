#include "groupstate.h"


GroupState::GroupState(QState *parent)
    : LinearState(parent)
{
}

void GroupState::onEntry(QEvent *event) {
    clearTransitions();
    addTransition(this, &QState::finished, getTargetState());
}