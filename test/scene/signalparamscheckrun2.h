#pragma once

#include <state/directstate.h>
#include <state/eventstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

#include "signalpsceventtrigger.h"

class WaitForTimesState : public EventState {
public:
    using EventState::EventState;

    int getSum() const {
        return dataSum;
    }

protected:
    void onEntry(QEvent *event) override {
        dataSum = 0;
        triggerSize = 3;
        EventState::onEntry(event);
    }

    bool testFinishBySignalData(const QVariantList &data) override {
        auto curValue = data.first().toInt();
        qDebug() << "trigger:" << 3 - triggerSize + 1 << "value:" << curValue;
        dataSum += curValue;
        return --triggerSize == 0;
    }

private:
    int dataSum = 0;
    int triggerSize = 0;
};

class SignalParamsCheckRun2 : public QObject {
public:
    static QStateMachine* run(SignalParamsCheckEventTrigger* trigger) {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        connect(beginState, &QState::entered, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto waitForTimesState = new WaitForTimesState(machine);
        waitForTimesState->setSignal(trigger, &SignalParamsCheckEventTrigger::trigger);
        connect(waitForTimesState, &QState::entered, machine, [&] {
            qDebug() << "wait for times state run...";
        });
        connect(waitForTimesState, &QState::exited, machine, [=] {
            qDebug() << "wait for times state exit..., data sum:" << waitForTimesState->getSum();
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        *beginState >> waitForTimesState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};