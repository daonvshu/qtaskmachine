#include "app.h"

#include <qfile.h>
#include <qmessagebox.h>

void App::refreshConfigPathLabel() {
    ui.config_path->setText(QStringLiteral("配置文件：%1").arg(configFilePath.isEmpty() ? QStringLiteral("（未设置）") : configFilePath));
}

void App::reloadFlowList() {
    ui.flow_list_cb->blockSignals(true);
    ui.flow_list_cb->clear();
    for (auto &flow : flowGroup.flows()) {
        ui.flow_list_cb->addItem(flow.name());
    }
    ui.flow_list_cb->blockSignals(false);

    auto currentIndex = ui.flow_list_cb->currentIndex();
    on_flow_list_cb_currentIndexChanged(currentIndex);

    updateFlowListWidth();
}

void App::createNewConfig(const QString &filePath) {
    configFilePath = filePath;
    flowGroup.flows().clear();
    refreshConfigPathLabel();
    reloadFlowList();
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
    refreshConfigPathLabel();
    reloadFlowList();
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
    auto data = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    file.write(data);
    file.close();
}

void App::updateFlowListWidth() {
    auto fm = ui.flow_list_cb->fontMetrics();
    auto maxWidth = 80;
    for (int i = 0; i < ui.flow_list_cb->count(); i++) {
        auto itemText = ui.flow_list_cb->itemText(i);
        maxWidth = qMax(maxWidth, fm.horizontalAdvance(itemText));
    }
    ui.flow_list_cb->setMinimumWidth(maxWidth + 20);
}
