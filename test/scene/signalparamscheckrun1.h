#pragma once

#include <state/directstate.h>
#include <state/eventstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

#include "signalpsceventtrigger.h"

class WaitForTargetValueState : public EventState {
public:
    explicit WaitForTargetValueState(int targetValue, QState* parent)
        : EventState(parent)
        , targetValue(targetValue)
    {}

protected:
    bool testFinishBySignalData(const QVariantList &data) override {
        int cur = data.first().toInt();
        bool valid = cur == targetValue;
        if (!valid) {
            qDebug() << "target value not match, value:" << cur << ", ignore...";
        }
        return valid;
    }

private:
    int targetValue;
};

class SignalParamsCheckRun1 : public QObject {
public:
    static QStateMachine* run(SignalParamsCheckEventTrigger* trigger) {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto waitForTargetState = new WaitForTargetValueState(10, machine);
        waitForTargetState->setSignal(trigger, &SignalParamsCheckEventTrigger::trigger);
        waitForTargetState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "wait for target state run...";
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        *beginState >> waitForTargetState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};