#pragma once

#include <qstate.h>
#include <functional>
#include <qtimer.h>

struct SelectState {
    QAbstractState* positive = nullptr;
    QAbstractState* negative = nullptr;

    std::function<bool()> condition = nullptr;
};

class TargetSignalTransition;
class LinearState : public QState {
    Q_OBJECT

public:
    explicit LinearState(QState* parent = nullptr);

    LinearState& operator>>(LinearState* nextState);

    void operator>>(QAbstractState* nextState);

    void setNext(QAbstractState* positive, QAbstractState* negative);

    void setCondition(const std::function<bool()>& condition);

    void setRunLoopSize(int size);

    void setStateTimeout(int ms);

    void setTimeoutRetrySize(int size);

signals:
    void stateRetry(QPrivateSignal);

protected:
    void clearTransitions();

    void onEntry(QEvent *event) override;

    void onExit(QEvent *event) override;

    enum TransitionType {
        IgnoreFeedbackWithDelay,
        CheckFeedbackWithTimeout,
        Directly,
        WaitForFinished,
        WaitForTarget,
        None,
    };

    virtual TransitionType getTransitionType();

    void timeoutCheck();

    virtual void timeoutHandler();

    virtual void onStateRetry();

    virtual TargetSignalTransition* getSignalTransition();

    virtual void signalArgumentsHandler(const QList<QVariant>& arguments);

protected:
    SelectState selectState;

    int delayMs;
    int loopCount = 0;
    int retryCount = 0;

    QTimer* timeoutCheckTimer;

protected:
    void addNewTargetTransition(TargetSignalTransition* transition);

    void addNewImmediatelyTransitionTarget();

    void addDelayToTargetTransition();

    void addWaitForSelfFinishTransition();

    QAbstractState* getTargetState();
};