#pragma once

#include <state/multieventstate.h>
#include <state/directstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

class MultiEventTrigger : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

signals:
    void event1();
    void event2();
    void event3();
};

class MultiEventStateRun : public QObject {
public:
    static QStateMachine* run(MultiEventTrigger* trigger) {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto eventState = new MultiEventState(machine);
        //设置监听信号
        eventState->addSignal(trigger, &MultiEventTrigger::event1);
        eventState->addSignal(trigger, &MultiEventTrigger::event2);
        eventState->addSignal(trigger, &MultiEventTrigger::event3);
        //进入状态监听
        eventState->bindState(TaskStateType::State_Enter, machine, [&] {
            qDebug() << "event state entered...";
        });
        //退出状态监听
        eventState->bindState(TaskStateType::State_Exit, machine, [&] {
            qDebug() << "event state exited...";
        });

        auto event1State = new QState(machine);
        connect(event1State, &QState::entered, machine, [&] {
            qDebug() << "event1 state entered...";
        });

        auto event2State = new QState(machine);
        connect(event2State, &QState::entered, machine, [&] {
            qDebug() << "event2 state entered...";
        });

        auto event3State = new QState(machine);
        connect(event3State, &QState::entered, machine, [&] {
            qDebug() << "event3 state entered...";
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        *beginState >> eventState << event1State << event2State << event3State >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};