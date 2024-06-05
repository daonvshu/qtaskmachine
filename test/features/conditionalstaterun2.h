#pragma once

#include <state/directstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

class ConditionalStateRun2 : public QObject {
public:
    static QStateMachine* run(bool positive) {
        auto machine = new QStateMachine;

        auto positiveState = new DirectState(machine);
        positiveState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "positive state run...";
        });

        auto negativeState = new DirectState(machine);
        negativeState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "negative state run...";
        });

        auto selectState = new DirectState(machine);
        //延迟读取positive值
        selectState->setCondition([=] {
            return positive;
        });
        selectState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "select state run...";
        });

        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [=] {
            qDebug() << "begin state run...";
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        *beginState >> selectState << negativeState << positiveState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};