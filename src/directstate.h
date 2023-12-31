#pragma once

#include "linearstate.h"

#include <qobject.h>

class DirectState : public LinearState {
public:
    explicit DirectState(QState *parent = nullptr);

protected:
    TransitionType getTransitionType() override;

    TargetSignalTransition * getSignalTransition() override;
};
