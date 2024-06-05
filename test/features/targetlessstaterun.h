#pragma once

#include <state/directstate.h>
#include <state/eventstate.h>
#include <state/delaystate.h>
#include <state/groupstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

enum class TargetLessType {
    Type_Delay,
    Type_Event,
    Type_SubGroup,
};

class TargetLessEventTrigger : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

    TargetLessType targetLessType;

signals:
    void exit();
    void trigger();
};

class TargetLessStateRun : public QObject {
public:
    static QStateMachine* run(TargetLessEventTrigger* trigger) {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto targetLessGroup = new EventState(machine);
        targetLessGroup->setSignal(trigger, &TargetLessEventTrigger::exit);
        switch (trigger->targetLessType) {
            case TargetLessType::Type_Delay: { //延时类型
                auto delayState = new DelayState(1000, targetLessGroup);
                delayState->bindState(TaskStateType::State_Enter, [] {
                    qDebug() << "delay state enter...";
                });
                targetLessGroup->setInitialState(delayState);
            }
                break;
            case TargetLessType::Type_Event: { //事件触发类型
                auto eventState = new EventState(targetLessGroup);
                eventState->setSignal(trigger, &TargetLessEventTrigger::trigger);
                eventState->bindState(TaskStateType::State_Enter, [] {
                    qDebug() << "event state enter...";
                });
                targetLessGroup->setInitialState(eventState);
            }
                break;
            case TargetLessType::Type_SubGroup: { //子状态组类型
                auto groupState = new GroupState(targetLessGroup);
                groupState->bindState(TaskStateType::State_Enter, [] {
                    qDebug() << "group state enter...";
                });
                {
                    auto delayState = new DelayState(2000, groupState);
                    delayState->bindState(TaskStateType::State_Enter, [] {
                        qDebug() << "delay state in group state enter...";
                    });

                    auto finalState = new QFinalState(groupState);
                    connect(finalState, &QState::entered, machine, [&] {
                        qDebug() << "group state final...";
                    });

                    *delayState >> finalState;
                    groupState->setInitialState(delayState);
                }
                targetLessGroup->setInitialState(groupState);
            }
                break;
        }

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        *beginState >> targetLessGroup >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};