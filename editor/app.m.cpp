#include "app.h"

#include "dialogs/messagedlg.h"

#include "db/viewcentermanager.h"

#include <qfile.h>

void App::refreshConfigPathLabel() {
    ui.config_path->setText(QStringLiteral("配置文件：%1").arg(flowGroup.configFilePath.isEmpty() ? QStringLiteral("（未设置）") : flowGroup.configFilePath));
}

void App::createNewConfig(const QString &filePath) {
    flowGroup.configFilePath = filePath;
    ViewCenterManager::beginFile(filePath);
    flowGroup.flows().clear();
    auto newFlow = ConfigFlow();
    newFlow.version = 2;
    newFlow.name = QStringLiteral("新流程");
    flowGroup.flows() << newFlow;
    refreshConfigPathLabel();
    ui.graphic_list->reloadTree();
}

void App::openExistConfig(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        MessageDlg::showMessage(QStringLiteral("错误"), QStringLiteral("文件无法打开！"), this);
        return;
    }
    auto data = file.readAll();
    auto doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        MessageDlg::showMessage(QStringLiteral("错误"), QStringLiteral("配置文件格式错误！"), this);
        return;
    }
    auto obj = doc.object();
    flowGroup.fromJson(obj);

    flowGroup.configFilePath = filePath;
    ViewCenterManager::beginFile(filePath);
    refreshConfigPathLabel();
    ui.graphic_list->reloadTree();
}

void App::bindRemoteState() {
    connect(&remoteControl, &RemoteControl::receiveActiveNode, this, [&] (const ReceiveActiveNode &activeNode) {
        ui.graphic_view->makeStateRunning(activeNode.flowName(), activeNode.nodeId());
    });

    connect(&remoteControl, &RemoteControl::disconnected, this, [&] {
        ui.graphic_view->clearRunningState();
    });

    connect(&remoteControl, &RemoteControl::stateRefreshed, this, [&] {
        auto currentFlow = ui.graphic_list->getCurrentSelectedFlow();
        if (currentFlow == nullptr) {
            return;
        }
        auto currentFlowName = currentFlow->name();
        auto currentFlowState = remoteControl.getFlowState(currentFlowName);
        if (currentFlowState.flowName().isEmpty()) {
            return;
        }
        ui.graphic_view->makeStateRunning(currentFlowName, currentFlowState.currentRunId());
    });
}