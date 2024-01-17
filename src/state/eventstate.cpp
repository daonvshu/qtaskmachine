#include "eventstate.h"

#include <qtimer.h>

EventState::EventState(QState *parent)
    : LinearState(parent)
    , signalTransition(nullptr)
    , failState(nullptr)
    , retryCount(0)
    , curRetrySize(-1)
    , timeoutMs(0)
    , transferBySelf(false)
{
    timeoutCheckTimer = new QTimer(this);
    timeoutCheckTimer->setSingleShot(true);

    connect(timeoutCheckTimer, &QTimer::timeout, this, &EventState::timeoutHandler);
}

void EventState::setTimeout(int ms) {
    timeoutMs = ms;
}

void EventState::setRetrySize(int count) {
    retryCount = count;
}

void EventState::setFailState(QAbstractState *state) {
    failState = state;
}

void EventState::onEntry(QEvent *event) {
    clearTransitions();

    if (!transferBySelf) {
        curRetrySize = retryCount;
    } else {
        transferBySelf = false;
    };

    Q_ASSERT(signalTransition != nullptr);

    //目标信号触发转移
    signalTransition->setTargetState(getTargetState());
    signalTransition->setSignalDataHandler([&] (const QVariantList& data) {
        return testFinishBySignalData(data);
    });
    addTransition(signalTransition);

    //失败信号转移
    if (failState != nullptr) {
        addTransition(this, &EventState::triggerSignalFail, failState);
    }

    //重试信号转移
    addTransition(this, &EventState::stateRetry, this);

    //子状态转移
    if (hasChild) {
        addTransition(this, &QState::finished, getTargetState());
    }

    //超时检测开始
    if (timeoutMs != 0) {
        timeoutCheckTimer->start(timeoutMs);
    }
}

void EventState::onExit(QEvent *event) {
    if (timeoutCheckTimer->isActive()) {
        timeoutCheckTimer->stop();
    }
}

bool EventState::testFinishBySignalData(const QVariantList &data) {
    return true;
}

void EventState::timeoutHandler() {
    if (--curRetrySize >= 0) {
        transferBySelf = true;
        stateRetry(QPrivateSignal());
    } else {
        emit triggerSignalFail();
    }
}
