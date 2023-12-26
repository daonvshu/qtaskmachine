#pragma once

#include "linearstate.h"

#include <qobject.h>

class GroupState : public LinearState
{
public:
    explicit GroupState(QState *parent = nullptr);

protected:
    TargetSignalTransition * getSignalTransition() override;

    TransitionType getTransitionType() override;
};