#pragma once

#include <qobject.h>

#include "utils/taskmachinesteputil.h"
#include "utils/taskmachinerunner.h"

#include "data/configflows.h"

class FlowStateRunner : public QObject {
    Q_OBJECT

    Q_PROPERTY(int __flow_debug_stateId MEMBER currentStateId NOTIFY stateIdChanged)

public:
    explicit FlowStateRunner(QObject *parent = nullptr);

    void run(int configIndex, const QString& configName, const ConfigFlowGroup& flowGroup);

    void cancel();

signals:
    void stateIdChanged();

private:
    TaskMachineRunner* taskMachineRunner = nullptr;
    ConfigFlowGroup tmpFlowGroupData;

    QHash<int, QString> enterFunctionMap, exitFunctionMap;

    int currentStateId = -1;

private:
    void createSignalMap(ConfigFlow& flow);
    void saveAndStep();

private slots:
    void onStateEnter();
    void onStateExit();
};
