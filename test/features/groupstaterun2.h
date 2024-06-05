#pragma once

#include <state/eventstate.h>
#include <state/directstate.h>
#include <state/delaystate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

class GroupEventTrigger : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

signals:
    void finish();
    void fail();
};

class GroupStateRun2 : public QObject {
public:
    static QStateMachine* run(GroupEventTrigger* trigger) {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto failState = new DirectState(machine);
        failState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "fail state run...";
        });

        auto groupState = new EventState(machine);
        //触发finish直接结束子状态，并切换到组状态下一个状态
        groupState->setSignal(trigger, &GroupEventTrigger::finish);
        //主动触发失败，从子状态直接切换到失败状态
        connect(trigger, &GroupEventTrigger::fail, machine, [=] {
            groupState->triggerSignalFail();
        });
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

            auto s2 = new DelayState(5000, groupState);
            s2->bindState(TaskStateType::State_Enter, machine, [&] {
                qDebug() << "group s2 enter...";
            });
            s2->bindState(TaskStateType::State_Exit, machine, [&] {
                qDebug() << "group s2 exited...";
            });

            auto finalState = new QFinalState(groupState);
            connect(finalState, &QState::entered, machine, [&] {
                qDebug() << "group final enter...";
            });

            //状态切换
            *s1 >> s2 >> finalState;

            groupState->setInitialState(s1);
        }

        auto nextState = new DirectState(machine);
        nextState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "next state enter...";
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        groupState->setFailState(failState);
        *beginState >> groupState >> nextState >> finalState;
        *failState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};