#pragma once

#include "linearstate.h"

#include <qobject.h>

class GroupState : public LinearState
{
public:
    explicit GroupState(QState *parent = nullptr);

protected:
    void onEntry(QEvent *event) override;
};