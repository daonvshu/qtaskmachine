#include "flowstaterunner.h"

#include <qstandardpaths.h>
#include <qdir.h>
#include <qdebug.h>

FlowStateRunner::FlowStateRunner(QObject *parent)
    : QObject(parent)
{
}

void FlowStateRunner::run(int configIndex, const QString &configName, const ConfigFlowGroup& flowGroup) {
    tmpFlowGroupData = flowGroup;
    createSignalMap(tmpFlowGroupData.flows()[configIndex]);
    saveAndStep();

    delete taskMachineRunner;
    taskMachineRunner = new TaskMachineRunner(configName, this);
    connect(taskMachineRunner, &TaskMachineRunner::finished, this, [] {
        qDebug() << "task runner finished!";
    });
    taskMachineRunner->run(this);
}

void FlowStateRunner::cancel() {
    if (taskMachineRunner) {
        taskMachineRunner->cancel();
        delete taskMachineRunner;
        taskMachineRunner = nullptr;
    }
}

void FlowStateRunner::createSignalMap(ConfigFlow &flow) {
    for (auto& executor : flow.executors()) {
        if (!executor.enter().isEmpty()) {
            enterFunctionMap[executor.id()] = executor.enter();
        }
        if (!executor.exit().isEmpty()) {
            exitFunctionMap[executor.id()] = executor.exit();
        }
        executor.enter = "onStateEnter()";
        executor.exit = "onStateExit()";

        ConfigFlowPropertyBind stateIdListenProp;
        stateIdListenProp.callOnEntered = true;
        stateIdListenProp.key = "__flow_debug_stateId";
        stateIdListenProp.value = QString::number(executor.id());
        stateIdListenProp.valueType = "int";
        executor.properties() << stateIdListenProp;
    }
}

void FlowStateRunner::saveAndStep() {
    auto cacheFilePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir path(cacheFilePath);
    if (!path.exists()) {
        path.mkpath(cacheFilePath);
    }
    auto tmpFile = path.filePath("tmp_flow_state_runner.json");
    QFile file(tmpFile);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    auto obj = tmpFlowGroupData.dumpToJson();
    auto data = QJsonDocument(obj).toJson(QJsonDocument::Indented);
    file.write(data);
    file.close();
    TaskMachineStepUtil::stepConfig(tmpFile);
}

void FlowStateRunner::onStateEnter() {
    qDebug() << "state" << currentStateId << "enter, call function:" << enterFunctionMap[currentStateId];
}

void FlowStateRunner::onStateExit() {
    qDebug() << "state" << currentStateId << "exit, call function:" << exitFunctionMap[currentStateId];
}


