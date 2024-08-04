#include "app.h"

#include "flowchart/elements/fcexecutoritem.h"
#include "flowchart/elements/fcbranchitem.h"
#include "flowchart/elements/fcconnectline.h"

#include <qfile.h>
#include <qmessagebox.h>

void App::itemNameChanged(const QModelIndex &index, const QString &text) {
    flowGroup.flows()[index.row()].name = text;
    saveConfigToFile();
}

void App::flowItemRemove(int index) {
    flowsMode->removeRow(index);
    flowGroup.flows().removeAt(index);
    auto current = ui.flow_list->currentIndex();
    if (current.isValid()) {
        reloadFlowConfig(current.row());
    } else {
        ui.flow_view->clear();
    }
    saveConfigToFile();
}

void App::saveFlowConfig(const QList<QGraphicsItem *> &items) {
    auto index = ui.flow_list->currentIndex();
    if (!index.isValid()) {
        return;
    }

    ConfigFlow& data = flowGroup.flows()[index.row()];
    data.executors().clear();
    data.lines().clear();
    QHash<QGraphicsItem*, int> idMap;
    for (const auto& item : items) {
        if (auto executor = dynamic_cast<FcExecutorItem*>(item)) {
            ConfigFlowExecutor flowExecutor;
            //id
            flowExecutor.id = executor->itemData.flowId;
            idMap[item] = flowExecutor.id();
            //text
            flowExecutor.text = executor->itemData.text;
            //task id
            flowExecutor.taskId = executor->itemData.taskId;
            //x y
            flowExecutor.fromScenePos(executor->rect().topLeft());
            //type
            flowExecutor.fromType(FlowChartNodeType::Node_Normal);
            data.executors().append(flowExecutor);
        } else if (auto condition = dynamic_cast<FcConditionalItem*>(item)) {
            ConfigFlowExecutor flowConditional;
            //id
            flowConditional.id = condition->flowId;
            idMap[item] = flowConditional.id();
            //x y
            flowConditional.fromScenePos(condition->rect().topLeft());
            //type
            flowConditional.fromType(FlowChartNodeType::Node_Condition);
            data.executors().append(flowConditional);
        }
    }
    for (const auto& item : items) {
        if (auto line = dynamic_cast<FcConnectLine*>(item)) {
            ConfigFlowConnectLine connectLine;
            //from
            connectLine.connectFrom = idMap[line->connectFrom];
            connectLine.connectFromPIndex = line->connectFrom->getConnectPointIndex(line->ctl.targetBegin);
            Q_ASSERT(connectLine.connectFromPIndex() != -1);
            //to
            connectLine.connectTo = idMap[line->connectTo];
            connectLine.connectToPIndex = line->connectTo->getConnectPointIndex(line->ctl.targetEnd);
            Q_ASSERT(connectLine.connectToPIndex() != -1);
            //control point
            connectLine.fromControlPos(line->ctl.targetControl);
            data.lines().append(connectLine);
        }
    }
    saveConfigToFile();
}

void App::saveConfigToFile() {
    if (configFilePath.isEmpty()) {
        return;
    }

    QFile file(configFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QMessageBox::critical(nullptr, QStringLiteral("错误"), QStringLiteral("文件无法写入！"));
        return;
    }
    auto obj = flowGroup.dumpToJson();
    auto data = QJsonDocument(obj).toJson(QJsonDocument::Indented);
    file.write(data);
    file.close();
}

void App::refreshConfigPathLabel() {
    ui.label_config_file_path->setText(QStringLiteral("配置文件：%1").arg(configFilePath.isEmpty() ? QStringLiteral("（未设置）") : configFilePath));
}

void App::createNewConfig(const QString &filePath) {
    bool oldConfigIsSet = !configFilePath.isEmpty();
    configFilePath = filePath;

    if (!oldConfigIsSet) {
        saveConfigToFile();
    } else {
        flowGroup.flows().clear();
        reloadFlowList();
    }
    refreshConfigPathLabel();
}

void App::openExistConfig(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, QStringLiteral("错误"), QStringLiteral("文件无法打开！"));
        return;
    }
    auto data = file.readAll();
    auto doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        QMessageBox::critical(nullptr, QStringLiteral("错误"), QStringLiteral("配置文件格式错误！"));
        return;
    }
    auto obj = doc.object();
    flowGroup.fromJson(obj);

    configFilePath = filePath;
    reloadFlowList();
    refreshConfigPathLabel();
}

void App::reloadFlowList() {
    flowsMode->clear();
    for (const auto& config : flowGroup.flows()) {
        auto item = new QStandardItem(config.name());
        flowsMode->appendRow(item);
    }

    if (flowGroup.flows().isEmpty()) {
        ui.flow_view->clear();
    } else {
        ui.flow_list->setCurrentIndex(flowsMode->index(0, 0));
        reloadFlowConfig(0);
    }
}

void App::reloadFlowConfig(int rowIndex) {
    auto scene = ui.flow_view->getScene();
    scene->clear();

    const auto& flow = flowGroup.flows()[rowIndex];
    QHash<int, FlowChartExecutorItem*> idMap;
    for (const auto& executor : flow.executors()) {
        switch (executor.itemType()) {
            case FlowChartNodeType::Node_Normal: {
                auto item = new FcExecutorItem(FlowChartItemData(executor.text(), executor.taskId()));
                item->setTopLeftPos(executor.scenePos(), scene);
                scene->addItem(item);
                idMap[executor.id()] = item;
            }
                break;
            case FlowChartNodeType::Node_Condition: {
                auto item = new FcConditionalItem;
                item->setTopLeftPos(executor.scenePos(), scene);
                scene->addItem(item);
                idMap[executor.id()] = item;
            }
                break;
        }
    }

    for (const auto& line : flow.lines()) {
        auto connectLine = new FcConnectLine;
        auto beginItem = idMap[line.connectFrom()];
        Q_ASSERT(beginItem != nullptr);
        connectLine->ctl.targetBegin = beginItem->getConnectPoints().at(line.connectFromPIndex());
        auto endItem = idMap[line.connectTo()];
        Q_ASSERT(endItem != nullptr);
        connectLine->ctl.targetEnd = endItem->getConnectPoints().at(line.connectToPIndex());
        FlowChartScene::addConnectLineFor(beginItem, endItem, connectLine);
        connectLine->ctl.targetControl = line.controlPos();
        connectLine->refreshConnectPath();
        scene->addItem(connectLine);
    }
}

void App::nodeSelected(FlowChartExecutorItem *item) {
    if (item == nullptr) {
        ui.stackedWidget->setCurrentIndex(0);
        return;
    }


}
