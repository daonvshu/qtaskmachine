#pragma once

#include "linearstate.h"

#include <qobject.h>

class DelayState : public LinearState
{
public:
    explicit DelayState(int delayMs, QState *parent = nullptr);

protected:
    TargetSignalTransition * getSignalTransition() override;

    TransitionType getTransitionType() override;
};
