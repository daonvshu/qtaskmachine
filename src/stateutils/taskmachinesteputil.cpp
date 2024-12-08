#include "taskmachinesteputil.h"

#include <qfile.h>

#ifdef QTASK_MACHINE_REMOTE_DEBUG_ENABLED
#include "../remote/remotedebuglistener.h"
#endif

TaskMachineStepUtil &TaskMachineStepUtil::instance() {
    static TaskMachineStepUtil instance;
    return instance;
}

bool TaskMachineStepUtil::stepConfig(const QString &fileName, int remoteDebugHost) {
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

#ifdef QTASK_MACHINE_REMOTE_DEBUG_ENABLED
    TaskMachine::RemoteDebugListener::startService(remoteDebugHost);
#endif

    return true;
}

TaskMachine::ConfigFlow TaskMachineStepUtil::getConfigFlow(const QString &flowName) {
    return instance().stateFlows[flowName];
}
