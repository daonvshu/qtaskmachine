#pragma once

#include <state/directstate.h>

#include <qstatemachine.h>
#include <qfinalstate.h>

Q_DECLARE_LOGGING_CATEGORY(debugState)

class DebugStateRun : public QObject {
public:
    static QStateMachine* run() {
        auto machine = new QStateMachine;

        //状态定义
        auto beginState = new DirectState(machine);
        beginState->setStateName("beginState", debugState);

        auto secondState = new DirectState(machine);
        secondState->setStateName("secondState", debugState);

        auto finalState = new QFinalState(machine);

        //状态切换
        *beginState >> secondState >> finalState;

        //初始化并开始
        machine->setInitialState(beginState);
        machine->start();

        return machine;
    }
};