#pragma once

#include <state/groupstate.h>
#include <state/directstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

class GroupStateRun1 : public QObject {
public:
    static QStateMachine* run() {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto groupState = new GroupState(machine);
        groupState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "group state entered...";
        });
        groupState->bindState(TaskStateType::State_Exit, machine, [&] {
            qDebug() << "group state exited...";
        });

        {
            auto s1 = new DirectState(groupState);
            s1->bindState(TaskStateType::State_Enter, machine, [&] {
                qDebug() << "group s1 enter...";
            });

            auto s2 = new DirectState(groupState);
            s2->bindState(TaskStateType::State_Enter, machine, [&] {
                qDebug() << "group s2 enter...";
            });

            auto finalState = new QFinalState(groupState);
            connect(finalState, &QState::entered, machine, [&] {
                qDebug() << "group final enter...";
            });

            //状态切换
            *s1 >> s2 >> finalState;

            groupState->setInitialState(s1);
        }

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        *beginState >> groupState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};