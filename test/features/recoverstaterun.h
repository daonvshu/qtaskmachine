#pragma once

#include <state/eventstate.h>
#include <state/directstate.h>
#include <state/delaystate.h>
#include <state/groupstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>
#include <qhistorystate.h>

#include <qdebug.h>

class RecoverEventTrigger : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

signals:
    void trigger();
    void recoverMain();
    void recoverChild();
};

class RecoverStateRun : public QObject {
public:
    static QStateMachine* run(RecoverEventTrigger* trigger) {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto failState = new QState(machine);
        connect(failState, &QState::entered, machine, [&] {
            qDebug() << "fail state run...";
        });

        auto mainGroupState = new EventState(machine);
        //主动触发失败，从子状态直接切换到失败状态
        connect(trigger, &RecoverEventTrigger::trigger, machine, [=] {
            mainGroupState->triggerSignalFail();
        });

        //主状态组历史状态
        auto mainGroupHistory = new QHistoryState(mainGroupState);

        //子状态组历史状态
        auto childGroupHistory = new QHistoryState(QHistoryState::DeepHistory, mainGroupState);

        mainGroupState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "main group state entered...";
        });
        mainGroupState->bindState(TaskStateType::State_Exit, machine, [&] {
            qDebug() << "main group state exited...";
        });

        {
            auto s1 = new DirectState(mainGroupState);
            s1->bindState(TaskStateType::State_Enter, machine, [&] {
                qDebug() << "group s1 enter...";
            });
            //设置历史默认状态
            mainGroupHistory->setDefaultState(s1);
            childGroupHistory->setDefaultState(s1);

            auto s2 = new DelayState(5000, mainGroupState);
            s2->bindState(TaskStateType::State_Enter, machine, [&] {
                qDebug() << "group s2 enter...";
            });
            s2->bindState(TaskStateType::State_Exit, machine, [&] {
                qDebug() << "group s2 exited...";
            });

            auto childGroup = new GroupState(mainGroupState);
            childGroup->bindState(TaskStateType::State_Enter, machine, [&] {
                qDebug() << "child group enter...";
            });
            childGroup->bindState(TaskStateType::State_Exit, machine, [&] {
                qDebug() << "child group exited...";
            });

            {
                auto s11 = new DirectState(childGroup);
                s11->bindState(TaskStateType::State_Enter, machine, [&] {
                    qDebug() << "group s11 enter...";
                });

                auto s12 = new DelayState(5000, childGroup);
                s12->bindState(TaskStateType::State_Enter, machine, [&] {
                    qDebug() << "group s12 enter...";
                });
                s12->bindState(TaskStateType::State_Exit, machine, [&] {
                    qDebug() << "group s12 exited...";
                });

                auto finalState = new QFinalState(childGroup);
                connect(finalState, &QState::entered, machine, [&] {
                    qDebug() << "child group final enter...";
                });

                //状态切换
                *s11 >> s12 >> finalState;

                childGroup->setInitialState(s11);
            }

            auto finalState = new QFinalState(mainGroupState);
            connect(finalState, &QState::entered, machine, [&] {
                qDebug() << "main group final enter...";
            });

            //状态切换
            *s1 >> s2 >> childGroup >> finalState;

            mainGroupState->setInitialState(s1);
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
        mainGroupState->setFailState(failState);
        failState->addTransition(trigger, &RecoverEventTrigger::recoverMain, mainGroupHistory);
        failState->addTransition(trigger, &RecoverEventTrigger::recoverChild, childGroupHistory);

        *beginState >> mainGroupState >> nextState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};