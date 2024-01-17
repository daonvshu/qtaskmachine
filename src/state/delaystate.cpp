#include "delaystate.h"

#include <qtimer.h>

DelayState::DelayState(int delayMs, QState *parent)
    : LinearState(parent)
    , delayMs(delayMs)
{
}

void DelayState::onEntry(QEvent *event) {
    clearTransitions();

    if (delayMs <= 0) {
        addTransition(getTargetState());
    } else {
        auto timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(delayMs);

        addTransition(timer, &QTimer::timeout, getTargetState());
        timer->start();
    }
}

void DelayState::setDelay(int ms) {
    delayMs = ms;
}
