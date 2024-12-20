#pragma once

#include <qobject.h>
#include <qhash.h>

#include "configflows.h"

class TaskMachineStepUtil {
public:
    static bool stepConfig(const QString& fileName, int remoteDebugHost = 5678);

    static TaskMachine::ConfigFlow getConfigFlow(const QString& flowName);

private:
    static TaskMachineStepUtil& instance();

private:
    QHash<QString, TaskMachine::ConfigFlow> stateFlows;
};
