#include "linearstate.h"

#include "targetsignaltransition.h"

#include <qdebug.h>

LinearState::LinearState(QState *parent)
    : QState(parent)
    , delayMs(10000)
{
    timeoutCheckTimer = new QTimer(this);
    timeoutCheckTimer->setSingleShot(true);
    connect(timeoutCheckTimer, &QTimer::timeout, this, &LinearState::timeoutCheck);
}

LinearState &LinearState::operator>>(LinearState *nextState) {
    selectState.positive = nextState;
    return *nextState;
}

void LinearState::operator>>(QAbstractState *nextState) {
    selectState.positive = nextState;
}

void LinearState::setNext(QAbstractState *positive, QAbstractState *negative) {
    selectState.positive = positive;
    selectState.negative = negative;
}

void LinearState::setTimeoutNext(QAbstractState *timeoutState) {
    selectState.timeout = timeoutState;
}

void LinearState::setCondition(const std::function<bool()> &condition) {
    selectState.condition = condition;
}

void LinearState::setInstantCondition(bool condition) {
    setCondition([=] {
        return condition;
    });
}

void LinearState::setRunLoopSize(int size) {
    loopCount = size;
}

void LinearState::setStateTimeout(int ms) {
    delayMs = ms;
}

void LinearState::setTimeoutRetrySize(int size) {
    retryCount = size;
}

void LinearState::clearTransitions() {
    auto oldTransitions = transitions();
    for (auto tr : oldTransitions) {
        removeTransition(tr);
    }
}

void LinearState::onEntry(QEvent *event) {
    switch (getTransitionType()) {
        case IgnoreFeedbackWithDelay:
            addDelayToTargetTransition();
            break;
        case CheckFeedbackWithTimeout:
            Q_ASSERT(delayMs != 0);
            timeoutCheckTimer->start(delayMs);
            addNewTargetTransition(getSignalTransition());
            if (selectState.timeout != nullptr && retryCount == 0) {
                addTransition(timeoutCheckTimer, &QTimer::timeout, selectState.timeout);
            }
            break;
        case Directly:
            addNewImmediatelyTransitionTarget();
            break;
        case WaitForFinished:
            addWaitForSelfFinishTransition();
            break;
        case WaitForTarget:
            addNewTargetTransition(getSignalTransition());
            break;
        case None:
            break;
    }
    if (retryCount > 0) {
        addTransition(this, &LinearState::stateRetry, this);
    }
}

void LinearState::onExit(QEvent *event) {
    if (timeoutCheckTimer->isActive()) {
        timeoutCheckTimer->stop();
    }
    clearTransitions();
}

LinearState::TransitionType LinearState::getTransitionType() {
    return None;
}

void LinearState::timeoutCheck() {
    if (--retryCount > 0) {
        onStateRetry();
        stateRetry(QPrivateSignal());
    } else {
        emit stateTimeout();
        timeoutHandler();
    }
}

void LinearState::timeoutHandler() {
}

void LinearState::onStateRetry() {
}

TargetSignalTransition *LinearState::getSignalTransition() {
    return nullptr;
}

void LinearState::signalArgumentsHandler(const QList<QVariant>&) {
    timeoutCheckTimer->start(delayMs); //reset timeout
}

void LinearState::addNewTargetTransition(TargetSignalTransition *transition) {
    clearTransitions();

    Q_ASSERT(selectState.positive != nullptr);
    connect(transition, &TargetSignalTransition::signalTriggered, this, &LinearState::signalArgumentsHandler);

    transition->setTargetState(getTargetState());
    transition->setSignalTriggeredSize(loopCount);
    addTransition(transition);
}

void LinearState::addNewImmediatelyTransitionTarget() {
    clearTransitions();

    Q_ASSERT(selectState.positive != nullptr);
    addTransition(getTargetState());
}

void LinearState::addDelayToTargetTransition() {
    clearTransitions();

    Q_ASSERT(selectState.positive != nullptr);

    if (delayMs <= 0) {
        addNewImmediatelyTransitionTarget();
    } else {
        auto timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(delayMs);

        addTransition(timer, &QTimer::timeout, getTargetState());
        timer->start();
    }
}

void LinearState::addWaitForSelfFinishTransition() {
    addTransition(this, &QState::finished, getTargetState());
}

QAbstractState *LinearState::getTargetState() {
    if (selectState.condition != nullptr) {
        return selectState.condition() ? selectState.positive : selectState.negative;
    } else {
        return selectState.positive;
    }
}
