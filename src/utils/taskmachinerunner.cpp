#include "taskmachinerunner.h"

#include "taskmachinesteputil.h"

#include "state/directstate.h"
#include "state/delaystate.h"
#include "state/eventstate.h"

#include <qfinalstate.h>
#include <qhistorystate.h>

#include <qloggingcategory.h>
#include <qdebug.h>

Q_LOGGING_CATEGORY(taskMachine, "task_machine")

using namespace TaskMachine;
TaskMachineRunner::TaskMachineRunner(const QString& flowName, QObject *parent)
    : QObject(parent)
{
    configFlow = TaskMachineStepUtil::getConfigFlow(flowName);
}

TaskMachine::ConfigFlow &TaskMachineRunner::getCurrentFlow() {
    return configFlow;
}

bool TaskMachineRunner::run(QObject *context) {
    currentBindContext = context;
    stepStateMachine();

    if (currentStateMachine) {
        currentStateMachine->start();
    }

    return currentStateMachine != nullptr;
}

void TaskMachineRunner::cancel() {
    delete currentStateMachine;
    currentStateMachine = nullptr;
}

void TaskMachineRunner::stepStateMachine() {
    delete currentStateMachine;
    currentStateMachine = nullptr;

    executors.clear();
    fromLines.clear();
    toLines.clear();
    createdState.clear();

    const TaskMachine::ConfigFlowExecutor* beginExecutor = nullptr;
    for (const ConfigFlowExecutor& executor : configFlow.executors()) {
        executors[executor.id()] = &executor;
        if (FlowChartNodeType(executor.type()) == FlowChartNodeType::Node_Begin) {
            beginExecutor = &executor;
        }
    }

    if (beginExecutor == nullptr) {
        return;
    }

    for (const ConfigFlowConnectLine& line : configFlow.lines()) {
        fromLines.insert(line.connectFrom(), &line);
        toLines.insert(line.connectTo(), &line);
    }

    currentStateMachine = new QStateMachine(this);
    connect(currentStateMachine, &QStateMachine::finished, this, &TaskMachineRunner::finished);
    auto beginState = createStateByType(beginExecutor, currentStateMachine);
    currentStateMachine->setInitialState(beginState);
}

QAbstractState* TaskMachineRunner::createStateByType(const TaskMachine::ConfigFlowExecutor *executor, QState* parent) {
    if (createdState.contains(executor->id())) {
        return createdState[executor->id()];
    }
    switch (FlowChartNodeType(executor->type())) {
        case FlowChartNodeType::Node_Begin:
        case FlowChartNodeType::Node_Normal:
            return createDirectState(executor, parent);
        case FlowChartNodeType::Node_Delay:
            return createDelayState(executor, parent);
        case FlowChartNodeType::Node_Event:
            return createEventState(executor, parent);
        case FlowChartNodeType::Node_MultiEvent:
            return createMultiEventState(executor, parent);
        case FlowChartNodeType::Node_Condition:
            return createConditionState(executor, parent);
        case FlowChartNodeType::Node_Group:
            return createGroupState(executor, parent);
        case FlowChartNodeType::Node_History:
            return createHistoryState(executor, parent);
        case FlowChartNodeType::Node_End:
            return createEndState(executor, parent);
    }
    return nullptr;
}

QAbstractState* TaskMachineRunner::createDirectState(const TaskMachine::ConfigFlowExecutor* executor, QState* parent) {
    auto state = new DirectState(parent);
    state->setStateName(executor->text(), taskMachine);
    createdState[executor->id()] = state;

    if (!executor->enter().isEmpty()) {
        state->bindState(TaskStateType::State_Enter, [this, executor] {
            invokeFunction(executor->enter());
        });
    }

    if (!executor->exit().isEmpty()) {
        state->bindState(TaskStateType::State_Exit, [this, executor] {
            invokeFunction(executor->exit());
        });
    }

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return state;
    }
    auto connectLine = connectLines.first();
    auto nextExecutor = executors[connectLine->connectTo()];
    if (nextExecutor == nullptr) {
        return state;
    }
    auto nextState = createStateByType(nextExecutor, parent);
    state->next(nextState);
    return state;
}

QAbstractState* TaskMachineRunner::createDelayState(const TaskMachine::ConfigFlowExecutor *executor, QState* parent) {
    auto state = new DelayState(executor->delay(), parent);
    state->setStateName(executor->text(), taskMachine);
    createdState[executor->id()] = state;

    if (!executor->enter().isEmpty()) {
        state->bindState(TaskStateType::State_Enter, [this, executor] {
            invokeFunction(executor->enter());
        });
    }

    if (!executor->exit().isEmpty()) {
        state->bindState(TaskStateType::State_Exit, [this, executor] {
            invokeFunction(executor->exit());
        });
    }

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return state;
    }
    auto connectLine = connectLines.first();
    auto nextExecutor = executors[connectLine->connectTo()];
    if (nextExecutor == nullptr) {
        return state;
    }
    auto nextState = createStateByType(nextExecutor, parent);
    state->next(nextState);
    return state;
}

QAbstractState* TaskMachineRunner::createEventState(const TaskMachine::ConfigFlowExecutor *executor, QState *parent) {
    auto state = new BindableCheckEventState(parent);
    state->setStateName(executor->text(), taskMachine);
    createdState[executor->id()] = state;

    if (!executor->enter().isEmpty()) {
        state->bindState(TaskStateType::State_Enter, [this, executor] {
            invokeFunction(executor->enter());
        });
    }

    if (!executor->exit().isEmpty()) {
        state->bindState(TaskStateType::State_Exit, [this, executor] {
            invokeFunction(executor->exit());
        });
    }

    state->setTimeout(executor->timeout());
    state->setRetrySize(executor->retry());

    if (!executor->funcRetry().isEmpty()) {
        connect(state, &EventState::stateRetry, [this, executor] {
            invokeFunction(executor->funcRetry());
        });
    }

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return state;
    }

    for (const auto connectLine : connectLines) {
        auto nextExecutor = executors[connectLine->connectTo()];
        if (nextExecutor == nullptr) {
            continue;
        }
        auto nextState = createStateByType(nextExecutor, parent);
        auto triggerFunc = findFunction(connectLine->trigger());
        if (!connectLine->failBranch()) {
            if (triggerFunc.isValid()) {
                state->setSignal(currentBindContext, triggerFunc.methodSignature().constData());
            }
            state->setCheckFunction(findFunction(connectLine->checkFunc()));
            state->next(nextState);
        } else {
            if (triggerFunc.isValid()) {
                connect(currentBindContext, triggerFunc, state, QMetaMethod::fromSignal(&EventState::triggerSignalFail));
            }
            state->setFailState(nextState);
        }
    }
    return state;
}

QAbstractState *TaskMachineRunner::createMultiEventState(const TaskMachine::ConfigFlowExecutor *executor, QState *parent) {
    auto state = new BindableMultiCheckEventState(parent);
    state->setStateName(executor->text(), taskMachine);
    createdState[executor->id()] = state;

    if (!executor->enter().isEmpty()) {
        state->bindState(TaskStateType::State_Enter, [this, executor] {
            invokeFunction(executor->enter());
        });
    }

    if (!executor->exit().isEmpty()) {
        state->bindState(TaskStateType::State_Exit, [this, executor] {
            invokeFunction(executor->exit());
        });
    }

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return state;
    }

    int signalIndex = 0;
    for (const auto connectLine : connectLines) {
        auto nextExecutor = executors[connectLine->connectTo()];
        if (nextExecutor == nullptr) {
            continue;
        }
        auto nextState = createStateByType(nextExecutor, parent);
        auto triggerFunc = findFunction(connectLine->trigger());
        if (triggerFunc.isValid()) {
            state->addSignal(currentBindContext, triggerFunc.methodSignature().constData());
            state->next(nextState);
            state->setCheckFunction(signalIndex++, connectLine->branchId(), findFunction(connectLine->checkFunc()));
        }
    }
    return state;
}

QAbstractState *TaskMachineRunner::createConditionState(const TaskMachine::ConfigFlowExecutor *executor, QState *parent) {
    auto state = new DirectState(parent);
    state->setStateName(executor->text(), taskMachine);
    createdState[executor->id()] = state;

    if (!executor->enter().isEmpty()) {
        state->bindState(TaskStateType::State_Enter, [this, executor] {
            invokeFunction(executor->enter());
        });
    }

    if (!executor->exit().isEmpty()) {
        state->bindState(TaskStateType::State_Exit, [this, executor] {
            invokeFunction(executor->exit());
        });
    }

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return state;
    }
    int signalIndex = 0;
    QHash<int, int> idToIndexMap;
    for (const auto connectLine : connectLines) {
        auto nextExecutor = executors[connectLine->connectTo()];
        if (nextExecutor == nullptr) {
            continue;
        }
        auto nextState = createStateByType(nextExecutor, parent);
        state->next(nextState);
        idToIndexMap[connectLine->branchId()] = signalIndex++;
    }
    state->setCondition([=] {
        auto checkFunc = findFunction(executor->condition());
        int branchId = 0;
        auto returnValue = Q_ARG(bool, branchId);
        bool invokeResult = checkFunc.invoke(this, Qt::DirectConnection, returnValue);
        if (!invokeResult) {
            qCCritical(taskMachine) << "invoke check function fail!";
            return 0;
        }
        return idToIndexMap[branchId];
    });

    return state;
}

QAbstractState *TaskMachineRunner::createGroupState(const TaskMachine::ConfigFlowExecutor *executor, QState *parent) {
    auto state = new EventState(parent);
    state->setStateName(executor->text(), taskMachine);
    createdState[executor->id()] = state;

    if (!executor->enter().isEmpty()) {
        state->bindState(TaskStateType::State_Enter, [this, executor] {
            invokeFunction(executor->enter());
        });
    }

    if (!executor->exit().isEmpty()) {
        state->bindState(TaskStateType::State_Exit, [this, executor] {
            invokeFunction(executor->exit());
        });
    }

    auto connectLines = fromLines.values(executor->id());
    if (connectLines.isEmpty()) {
        return state;
    }

    for (const auto connectLine : connectLines) {
        auto nextExecutor = executors[connectLine->connectTo()];
        if (nextExecutor == nullptr) {
            continue;
        }

        if (connectLine->failBranch()) {
            auto nextState = createStateByType(nextExecutor, parent);
            auto triggerFunc = findFunction(connectLine->trigger());
            if (triggerFunc.isValid()) {
                connect(currentBindContext, triggerFunc, state, QMetaMethod::fromSignal(&EventState::triggerSignalFail));
            }
            state->setFailState(nextState);
        } else if (connectLine->subBranch()) {
            auto nextState = createStateByType(nextExecutor, state);
            state->setInitialState(nextState);
        } else {
            auto nextState = createStateByType(nextExecutor, parent);
            state->next(nextState);
        }
    }
    return state;
}

QAbstractState *TaskMachineRunner::createHistoryState(const TaskMachine::ConfigFlowExecutor *executor, QState *parent) {
    auto state = new QHistoryState(parent);
    createdState[executor->id()] = state;

    if (executor->nested()) {
        state->setHistoryType(QHistoryState::DeepHistory);
    }

    if (!executor->enter().isEmpty()) {
        connect(state, &QHistoryState::entered, [this, executor] {
            qCInfo(taskMachine) << "state '" << executor->text() << "' enter!";;
            invokeFunction(executor->enter());
        });
    }

    if (!executor->exit().isEmpty()) {
        connect(state, &QHistoryState::exited, [this, executor] {
            qCInfo(taskMachine) << "state '" << executor->text() << "' exit!";;
            invokeFunction(executor->exit());
        });
    }

    return state;
}

QAbstractState *TaskMachineRunner::createEndState(const TaskMachine::ConfigFlowExecutor *executor, QState *parent) {
    auto state = new QFinalState(parent);
    createdState[executor->id()] = state;

    if (!executor->enter().isEmpty()) {
        connect(state, &QFinalState::entered, [this, executor] {
            qCInfo(taskMachine) << "state '" << executor->text() << "' enter!";;
            invokeFunction(executor->enter());
        });
    }

    if (!executor->exit().isEmpty()) {
        connect(state, &QFinalState::exited, [this, executor] {
            qCInfo(taskMachine) << "state '" << executor->text() << "' exit!";;
            invokeFunction(executor->exit());
        });
    }

    return state;
}

void TaskMachineRunner::invokeFunction(const QString &slotName) {
    if (slotName.isEmpty()) {
        return;
    }
    auto functionName = slotName.toLatin1();
    int methodIndex = currentBindContext->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(functionName));
    if (methodIndex == -1) {
        qCCritical(taskMachine) << "cannot find slot function:" << slotName << "in context:" << currentBindContext->metaObject()->className();
        return;
    }
    auto method = currentBindContext->metaObject()->method(methodIndex);
    method.invoke(currentBindContext, Qt::DirectConnection);
}

QMetaMethod TaskMachineRunner::findFunction(const QString &signalName) {
    if (signalName.isEmpty()) {
        return {};
    }
    auto functionName = signalName.toLatin1();
    int methodIndex = currentBindContext->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(functionName));
    if (methodIndex == -1) {
        qCCritical(taskMachine) << "cannot find function:" << signalName << "in context:" << currentBindContext->metaObject()->className();
        return {};
    }
    return currentBindContext->metaObject()->method(methodIndex);
}


bool BindableCheckEventState::testFinishBySignalData(const QVariantList &data)  {
    if (checkFunction.isValid()) {
        bool boolData = true;
        auto returnValue = Q_ARG(bool, boolData);
        bool invokeResult = checkFunction.invoke(this, Qt::DirectConnection, returnValue, Q_ARG(QVariantList, data));
        if (!invokeResult) {
            qCCritical(taskMachine) << "invoke check function fail!";
            return true;
        }
        return boolData;
    }
    return true;
}

bool BindableMultiCheckEventState::testFinishBySignalData(int signalIndex, const QVariantList &data)  {
    auto checkFunc = checkFunction[signalIndex];
    if (checkFunc.second.isValid()) {
        bool boolData = true;
        auto returnValue = Q_ARG(bool, boolData);
        bool invokeResult = checkFunc.second.invoke(this, Qt::DirectConnection, returnValue, Q_ARG(int, checkFunc.first), Q_ARG(QVariantList, data));
        if (!invokeResult) {
            qCCritical(taskMachine) << "invoke check function fail!";
            return true;
        }
        return boolData;
    }
    return true;
}
