#pragma once

#include <state/directstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

class DirectStateRun : public QObject {
public:
    static QStateMachine* run() {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        connect(beginState, &QState::entered, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto secondState = new DirectState(machine);
        connect(secondState, &QState::entered, machine, [&] {
            qDebug() << "second state run...";
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        *beginState >> secondState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};

