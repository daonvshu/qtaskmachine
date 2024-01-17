#pragma once

#include <state/groupstate.h>
#include <state/directstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

class GroupStateRun : public QObject {
public:
    static QStateMachine* run() {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        connect(beginState, &QState::entered, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto groupState = new GroupState(machine);
        connect(groupState, &QState::entered, machine, [&] {
            qDebug() << "group state entered...";
        });
        connect(groupState, &QState::exited, machine, [&] {
            qDebug() << "group state exited...";
        });

        {
            auto s1 = new DirectState(groupState);
            connect(s1, &QState::entered, machine, [&] {
                qDebug() << "group s1 enter...";
            });

            auto s2 = new DirectState(groupState);
            connect(s2, &QState::entered, machine, [&] {
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