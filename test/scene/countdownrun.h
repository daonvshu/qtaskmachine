#pragma once

#include <state/directstate.h>
#include <state/eventstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qtimer.h>
#include <qdebug.h>

class CountDownState : public EventState {
    Q_OBJECT

public:
    explicit CountDownState(QState* parent = nullptr)
        : EventState(parent)
    {
        timer = new QTimer(this);
        timer->setSingleShot(false);
        timer->setInterval(1000);
        connect(timer, &QTimer::timeout, this, &CountDownState::countDownTimeout);

        setSignal(this, &CountDownState::countDownFinished);
    }

    void setTime(int from, int to) {
        secFrom = from;
        secTo = to;
    }

    void pause() {
        timer->stop();
    }

    void resume() {
        timer->start();
    }

    void skip() {
        secCur = secTo + 1;
    }

signals:
    void countDownUpdate(int curSec);
    void countDownFinished(QPrivateSignal);

private:
    void countDownTimeout() {
        secCur--;
        emit countDownUpdate(secCur);
        if (secCur <= secTo) {
            timer->stop();
            emit countDownFinished(QPrivateSignal());
        }
    }

    void onEntry(QEvent *event) override {
        EventState::onEntry(event);
        secCur = secFrom;
        timer->start();
        emit countDownUpdate(secCur);
    }

private:
    QTimer* timer;
    int secFrom = 0, secTo = 0;
    int secCur = 0;
};

class CountDownEventTrigger : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

signals:
    void pause();
    void resume();
    void skip();
};

class CountDownRun : public QObject {
public:
    static QStateMachine* run(CountDownEventTrigger* trigger, int from, int to) {
        auto machine = new QStateMachine;

        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto countDownState = new CountDownState(machine);
        countDownState->setTime(from, to);
        connect(countDownState, &CountDownState::countDownUpdate, machine, [&] (int sec) {
            qDebug() << "count down update:" << sec;
        });
        connect(trigger, &CountDownEventTrigger::pause, countDownState, &CountDownState::pause);
        connect(trigger, &CountDownEventTrigger::resume, countDownState, &CountDownState::resume);
        connect(trigger, &CountDownEventTrigger::skip, countDownState, &CountDownState::skip);

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        *beginState >> countDownState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};