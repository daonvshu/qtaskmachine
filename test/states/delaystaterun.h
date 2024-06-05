#pragma once

#include <state/delaystate.h>
#include <state/directstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

class DelayStateRun : public QObject {
public:
    static QStateMachine* run(int delayMs) {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto delayState = new DelayState(delayMs, machine);
        delayState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "delay before...";
        });
        delayState->bindState(TaskStateType::State_Exit, machine, [&] {
            qDebug() << "delay after...";
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        *beginState >> delayState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};