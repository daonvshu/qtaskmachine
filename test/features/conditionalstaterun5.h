#pragma once

#include <state/directstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

class ConditionalStateRun5 : public QObject {
public:
    static QStateMachine* run(bool positive) {
        auto machine = new QStateMachine;

        auto positiveState = new DirectState(machine);
        connect(positiveState, &QState::entered, machine, [&] {
            qDebug() << "positive state run...";
        });

        auto negativeState = new DirectState(machine);
        connect(negativeState, &QState::entered, machine, [&] {
            qDebug() << "negative state run...";
        });

        auto beginState = new DirectState(machine);
        beginState->setConditionDeferrable(); //延迟切换
        beginState->setCondition([=] {
            return positive;
        });
        connect(beginState, &QState::entered, machine, [=] {
            qDebug() << "begin state run...";
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        *beginState << negativeState << positiveState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};