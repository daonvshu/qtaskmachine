#include "app.h"

#include "flowchart/elements/fcexecutoritem.h"
#include "flowchart/elements/fcbranchitem.h"
#include "flowchart/elements/fcconnectline.h"

#include "subpage/stateconfiginterface.h"

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
            flowExecutor.fromType(executor->itemData.nodeType);
            //function
            flowExecutor.enter = executor->itemData.functionEnter;
            flowExecutor.exit = executor->itemData.functionExit;
            //delay
            flowExecutor.delay = executor->itemData.delayMs;
            //event
            flowExecutor.timeout = executor->itemData.timeoutMs;
            flowExecutor.retry = executor->itemData.timeoutRetry;
            flowExecutor.funcRetry = executor->itemData.functionRetry;

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
            //data
            flowConditional.text = condition->conditionData.text;
            flowConditional.enter = condition->conditionData.functionEnter;
            flowConditional.exit = condition->conditionData.functionExit;
            flowConditional.condition = condition->conditionData.functionCondition;
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
            //data
            connectLine.trigger = line->lineData.functionTrigger;
            connectLine.checkFunc = line->lineData.functionCheck;
            connectLine.failBranch = line->lineData.failBranch;
            connectLine.branchId = line->lineData.branchId;
            connectLine.subBranch = line->lineData.subBranch;

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

    QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("OK!"));
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
            case FlowChartNodeType::Node_Condition: {
                auto item = new FcConditionalItem;
                item->setTopLeftPos(executor.scenePos(), scene);
                item->conditionData.text = executor.text();
                item->conditionData.functionEnter = executor.enter();
                item->conditionData.functionExit = executor.exit();
                item->conditionData.functionCondition = executor.condition();
                scene->addItem(item);
                idMap[executor.id()] = item;
            }
                break;
            default: {
                FlowChartItemData itemData(executor.text(), executor.taskId(), executor.itemType());
                itemData.functionEnter = executor.enter();
                itemData.functionExit = executor.exit();
                itemData.delayMs = executor.delay();
                itemData.timeoutMs = executor.timeout();
                itemData.timeoutRetry = executor.retry();
                itemData.functionRetry = executor.funcRetry();
                auto item = new FcExecutorItem(itemData);
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
        connectLine->lineData.functionTrigger = line.trigger();
        connectLine->lineData.functionCheck = line.checkFunc();
        connectLine->lineData.failBranch = line.failBranch();
        connectLine->lineData.branchId = line.branchId();
        connectLine->lineData.subBranch = line.subBranch();
        scene->addItem(connectLine);
    }
}

void App::nodeSelected(QGraphicsItem *item) {

    if (item == nullptr) {
        ui.stackedWidget->setCurrentIndex(0);
        ui.state_common_prop->setVisible(false);
        return;
    }

    if (auto executor = dynamic_cast<FcExecutorItem*>(item)) {
        auto nodeType = executor->itemData.nodeType;
        switch (nodeType) {
            case FlowChartNodeType::Node_Begin:
            case FlowChartNodeType::Node_End:
            case FlowChartNodeType::Node_Normal:
            case FlowChartNodeType::Node_Group:
            case FlowChartNodeType::Node_History:
                ui.stackedWidget->setCurrentIndex(0);
                break;
            case FlowChartNodeType::Node_Delay:
                ui.stackedWidget->setCurrentIndex(1);
                break;
            case FlowChartNodeType::Node_Event:
                ui.stackedWidget->setCurrentIndex(2);
                break;
            case FlowChartNodeType::Node_MultiEvent:
                ui.stackedWidget->setCurrentIndex(3);
                break;
            default:
                break;
        }
        ui.state_common_prop->setVisible(true);
        commonPropManager->loadItemBaseData(executor);
        auto i = dynamic_cast<StateConfigInterface*>(ui.stackedWidget->currentWidget());
        if (i) {
            i->setActiveItem(executor);
        }
    } else if (auto connectLine = dynamic_cast<FcConnectLine*>(item)) {
        ui.state_common_prop->setVisible(false);
        if (auto prevItem = dynamic_cast<FcExecutorItem*>(connectLine->connectFrom)) {
            switch (prevItem->itemData.nodeType) {
                case FlowChartNodeType::Node_Begin:
                case FlowChartNodeType::Node_End:
                case FlowChartNodeType::Node_Normal:
                case FlowChartNodeType::Node_Delay:
                case FlowChartNodeType::Node_History:
                    ui.stackedWidget->setCurrentIndex(0);
                    break;
                case FlowChartNodeType::Node_Event:
                    ui.stackedWidget->setCurrentIndex(2);
                    break;
                case FlowChartNodeType::Node_MultiEvent:
                    ui.stackedWidget->setCurrentIndex(3);
                    break;
                case FlowChartNodeType::Node_Group:
                    ui.stackedWidget->setCurrentIndex(5);
                    break;
                default:
                    break;
            }
            auto i = dynamic_cast<StateConfigInterface*>(ui.stackedWidget->currentWidget());
            if (i) {
                i->setActiveLine(connectLine);
            }

        } else if (auto prevCondition = dynamic_cast<FcConditionalItem*>(connectLine->connectFrom)) {
            ui.stackedWidget->setCurrentIndex(4);
            auto i = dynamic_cast<StateConfigInterface*>(ui.stackedWidget->currentWidget());
            if (i) {
                i->setActiveLine(connectLine);
            }
        } else {
            ui.stackedWidget->setCurrentIndex(0);
        }
    } else if (auto condition = dynamic_cast<FcConditionalItem*>(item)) {
        ui.stackedWidget->setCurrentIndex(4);
        ui.state_common_prop->setVisible(true);
        commonPropManager->loadItemBaseData(condition);
        auto i = dynamic_cast<StateConfigInterface*>(ui.stackedWidget->currentWidget());
        if (i) {
            i->setActiveItem(condition);
        }
    } else {
        qCritical() << "unexpected flowchart item type!";
    }
}
