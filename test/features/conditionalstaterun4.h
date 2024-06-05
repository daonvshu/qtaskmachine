#pragma once

#include <state/directstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

class ConditionalStateRun4 : public QObject {
public:
    static QStateMachine* run(bool positive) {
        auto machine = new QStateMachine;

        auto conditionalState = new DirectState(machine);
        conditionalState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "conditional state run...";
        });

        auto defaultState = new DirectState(machine);
        defaultState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "default state run...";
        });

        auto selectState = new DirectState(machine);
        selectState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "select state run...";
        });

        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [=] {
            qDebug() << "begin state run...";
            //在selectState进入前设置条件
            selectState->setCondition(positive);
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        *beginState >> selectState << defaultState << conditionalState >> defaultState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};