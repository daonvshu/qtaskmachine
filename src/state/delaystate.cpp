#include "delaystate.h"

#include <qtimer.h>

DelayState::DelayState(int delayMs, QState *parent)
    : LinearState(parent)
    , delayMs(delayMs)
{
}

DelayState::DelayState(const std::function<int()>& delayReader, QState* parent)
    : LinearState(parent)
    , delayMs(0)
    , delayReader(delayReader)
{
}

void DelayState::onEntry(QEvent *event) {
    LinearState::onEntry(event);

    clearTransitions();

    if (delayReader) {
        delayMs = delayReader();
    }
    if (delayMs <= 0) {
        auto target = getTargetState();
        Q_ASSERT(target != this);
        addTransition(target);
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
