#pragma once

#include <qobject.h>
#include <qhash.h>

#include "configflows.h"

class TaskMachineStepUtil {
public:
    static bool stepConfig(const QString& fileName
#ifdef QTASK_MACHINE_REMOTE_DEBUG_ENABLED
    , int remoteDebugHost = 5678
#endif
    );

    static TaskMachine::ConfigFlow getConfigFlow(const QString& flowName);

private:
    static TaskMachineStepUtil& instance();

private:
    QHash<QString, TaskMachine::ConfigFlow> stateFlows;
};
