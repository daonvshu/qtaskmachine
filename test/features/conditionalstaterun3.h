#pragma once

#include <state/directstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

class ConditionalStateRun3 : public QObject {
public:
    static QStateMachine* run(int targetIndex) {
        auto machine = new QStateMachine;

        auto s1 = new DirectState(machine);
        s1->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "s1 state run...";
        });

        auto s2 = new DirectState(machine);
        s2->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "s2 state run...";
        });

        auto s3 = new DirectState(machine);
        s3->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "s3 state run...";
        });

        auto s4 = new DirectState(machine);
        s4->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "s4 state run...";
        });

        auto selectState = new DirectState(machine);
        //延迟读取index值
        selectState->setCondition([=] {
            return targetIndex;
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

        *beginState >> selectState << s1 << s2 << s3 << s4 >> finalState;
        //selectState->next(s1).next(s2).next(s3).next(s4);
        //beginState->target(selectState).target(finalState);

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};