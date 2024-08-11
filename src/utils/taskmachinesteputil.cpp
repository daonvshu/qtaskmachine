#include "taskmachinesteputil.h"

#include <qfile.h>

TaskMachineStepUtil &TaskMachineStepUtil::instance() {
    static TaskMachineStepUtil instance;
    return instance;
}

bool TaskMachineStepUtil::stepConfig(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    auto data = file.readAll();
    file.close();
    auto jsonDoc = QJsonDocument::fromJson(data);
    if (jsonDoc.isNull()) {
        return false;
    }
    TaskMachine::ConfigFlowGroup flowGroup;
    flowGroup.fromJson(jsonDoc.object());

    instance().stateFlows.clear();
    for (const auto& flow : flowGroup.flows()) {
        instance().stateFlows.insert(flow.name(), flow);
    }

    return true;
}

TaskMachine::ConfigFlow TaskMachineStepUtil::getConfigFlow(const QString &flowName) {
    return instance().stateFlows[flowName];
}
