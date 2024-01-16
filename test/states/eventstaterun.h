#pragma once

#include <state/eventstate.h>
#include <state/directstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

#include <qdebug.h>

class EventTrigger : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

signals:
    void finish();
    void fail();
};

class EventStateRun : public QObject {
public:
    static QStateMachine* run(EventTrigger* trigger, int timeoutMs, int retryCount) {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        connect(beginState, &QState::entered, machine, [&] {
            qDebug() << "begin state run...";
        });

        auto failState = new DirectState(machine);
        connect(failState, &QState::entered, machine, [&] {
            qDebug() << "fail state enter...";
        });

        auto eventState = new EventState(machine);
        //设置监听信号
        eventState->setSignal(trigger, &EventTrigger::finish);
        //设置超时
        eventState->setTimeout(timeoutMs);
        //设置超时重试
        eventState->setRetrySize(retryCount);
        //设置失败切换状态
        eventState->setFailState(failState);
        //进入状态监听
        connect(eventState, &EventState::entered, machine, [&] {
            qDebug() << "event state entered...";
        });
        //退出状态监听
        connect(eventState, &EventState::exited, machine, [&] {
            qDebug() << "event state exited...";
        });
        //监听超时重试
        connect(eventState, &EventState::stateRetry, machine, [&] {
            qDebug() << "event state retry...";
        });
        //主动触发失败
        connect(trigger, &EventTrigger::fail, machine, [=] {
            eventState->triggerSignalFail();
        });

        auto finalState = new QFinalState(machine);
        connect(finalState, &QState::entered, machine, [&] {
            qDebug() << "final...";
        });

        //状态切换
        *beginState >> eventState >> finalState;
        *failState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};