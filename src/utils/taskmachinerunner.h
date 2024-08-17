#pragma once

#include <qobject.h>
#include <qstatemachine.h>

#include "configflows.h"

#include "state/eventstate.h"
#include "state/multieventstate.h"

class TaskMachineRunner : public QObject {
    Q_OBJECT

public:
    explicit TaskMachineRunner(const QString& flowName, QObject *parent = nullptr);

    TaskMachine::ConfigFlow& getCurrentFlow();

    bool run(QObject* context);

    void cancel();

signals:
    void finished();

private:
    TaskMachine::ConfigFlow configFlow;
    QObject* currentBindContext = nullptr;
    QStateMachine* currentStateMachine = nullptr;

    //tmp data
    QHash<int, const TaskMachine::ConfigFlowExecutor*> executors;
    QMultiHash<int, const TaskMachine::ConfigFlowConnectLine*> fromLines, toLines;
    QHash<int, QAbstractState*> createdState;

private:
    void stepStateMachine();

    QAbstractState* createStateByType(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createDirectState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createDelayState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createEventState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createMultiEventState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createConditionState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createGroupState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createHistoryState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);
    QAbstractState* createEndState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent);

    void bindExecutorBaseInfo(QAbstractState* state, const TaskMachine::ConfigFlowExecutor* executor, bool printLog = false);

    void invokeFunction(const QString& slotName);
    QMetaMethod findFunction(const QString& signalName);
};

class BindableCheckEventState : public EventState {
public:
    using EventState::EventState;

    void setCheckFunction(const QMetaMethod& function) {
        checkFunction = function;
    }

protected:
    bool testFinishBySignalData(const QVariantList &data) override;

private:
    QMetaMethod checkFunction;
};

class BindableMultiCheckEventState : public MultiEventState {
public:
    using MultiEventState::MultiEventState;

    void setCheckFunction(int signalIndex, int branchId, const QMetaMethod& function) {
        checkFunction[signalIndex] = qMakePair(branchId, function);
    }

protected:
    bool testFinishBySignalData(int signalIndex, const QVariantList &data) override;

private:
    QHash<int, QPair<int, QMetaMethod>> checkFunction;
};