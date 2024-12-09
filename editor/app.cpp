#include "app.h"

#include <QWKWidgets/widgetwindowagent.h>
#include <qdebug.h>
#include <qtimer.h>
#include <qfiledialog.h>
#include <qstandardpaths.h>
#include <qmimedata.h>
#include <qlistview.h>
#include <qaction.h>
#include <qmenu.h>

#include "appsettings.h"

#include "dialogs/flownameeditdlg.h"
#include "dialogs/messagedlg.h"
#include "remote/monitordlg.h"

App::App(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.flow_list_cb->setView(new QListView);

    auto agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);
    agent->setTitleBar(ui.app_title);
    agent->setHitTestVisible(ui.system_buttons);
    agent->setHitTestVisible(ui.flow_list);
    agent->setHitTestVisible(ui.btn_new_config);
    agent->setHitTestVisible(ui.btn_open_config);
    agent->setHitTestVisible(ui.btn_monitor);

    refreshConfigPathLabel();

    AppSettings::init();
    auto lastOpenFilePath = AppSettings::lastOpenFilePath();
    if (!lastOpenFilePath.isEmpty()) {
        if (!QFileInfo::exists(lastOpenFilePath)) {
            return;
        }
        QTimer::singleShot(200, [&, lastOpenFilePath] {
            openExistConfig(lastOpenFilePath);
        });
    }

    bindRemoteState();
}

void App::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    QTimer::singleShot(0, this, [=]() {
        ui.btn_max->setIcon(isMaximized() ? QIcon(":/res/maxsize2.svg") : QIcon(":/res/maxsize1.svg"));
    });
}

void App::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        QList<QUrl> urlList = event->mimeData()->urls();
        if (!urlList.isEmpty()) {
            QString filePath = urlList.first().toLocalFile();
            if (QFileInfo(filePath).suffix() == "json") {
                event->acceptProposedAction(); // 接受拖放
                return;
            }
        }
    }
    event->ignore();
}

void App::dropEvent(QDropEvent *event) {
    auto mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = event->mimeData()->urls();
        if (!urlList.isEmpty()) {
            auto filePath = urlList.first().toLocalFile();
            saveLastOpenFilePathRecord(filePath);
            openExistConfig(filePath);
        }
    }
}

void App::on_btn_min_clicked() {
    showMinimized();
}

void App::on_btn_max_clicked() {
    if (isMaximized()) {
        showNormal();
    } else {
        showMaximized();
    }
}

void App::on_btn_close_clicked() {
    close();
}

void App::on_btn_new_config_clicked() {
    auto openFilePath = configFilePath;
    if (openFilePath.isEmpty()) {
        openFilePath = AppSettings::lastOpenFilePath();
    }
    auto path = QFileDialog::getSaveFileName(nullptr, tr("创建配置"),
                                             openFilePath.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) : openFilePath, "*.json");
    if (path.isEmpty()) {
        return;
    }
    saveLastOpenFilePathRecord(path);
    createNewConfig(path);
}

void App::on_btn_open_config_clicked() {

    auto loadRecentFile = [&] (const QString& filePath) {
        if (!QFileInfo::exists(filePath)) {
            MessageDlg::showMessage(tr("提示"), tr("文件不存在：%1").arg(filePath), this);
            return;
        }
        saveLastOpenFilePathRecord(filePath);
        openExistConfig(filePath);
    };

    QMenu menu(ui.btn_open_config);
    for (const auto& filePath : AppSettings::recentFiles()) {
        menu.addAction(filePath, [=] {
            loadRecentFile(filePath);
        });
    }
    menu.addSeparator();
    menu.addAction(tr("打开新配置"), [&] {
        auto openFilePath = configFilePath;
        if (openFilePath.isEmpty()) {
            openFilePath = AppSettings::lastOpenFilePath();
        }
        auto path = QFileDialog::getOpenFileName(nullptr, tr("打开配置"),
                                                 openFilePath.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) : openFilePath, "*.json");
        if (path.isEmpty()) {
            return;
        }
        loadRecentFile(path);
    });
    menu.exec(ui.btn_open_config->mapToGlobal(QPoint(0, ui.btn_open_config->height())));
}

void App::on_flow_list_cb_currentIndexChanged(int index) {
    if (index == -1) {
        ui.graphic_view->updateFlow(nullptr);
        if (monitorDlg) {
            monitorDlg->flowChanged(nullptr);
        }
    } else {
        ui.graphic_view->updateFlow(&flowGroup.flows()[index]);
        if (monitorDlg) {
            monitorDlg->flowChanged(&flowGroup.flows()[index]);
        }
    }
}

void App::on_btn_flow_add_clicked() {
    auto newFlowName = FlowNameEditDlg::showDialog(QString(), tr("新建流程"), this);
    if (!newFlowName.isEmpty()) {
        ConfigFlow configFlow;
        configFlow.name = newFlowName;
        flowGroup.flows().append(configFlow);
        ui.flow_list_cb->addItem(newFlowName);
        ui.flow_list_cb->setCurrentIndex(ui.flow_list_cb->count() - 1);
        saveConfigToFile();
        updateFlowListWidth();
    }
}

void App::on_btn_flow_copy_clicked() {
    auto currentIndex = ui.flow_list_cb->currentIndex();
    if (currentIndex == -1) {
        return;
    }
    auto& currentFlow = flowGroup.flows()[currentIndex];
    auto newFlowName = FlowNameEditDlg::showDialog(currentFlow.name(), tr("复制流程"), this);
    if (!newFlowName.isEmpty()) {
        ConfigFlow configFlow = currentFlow;
        configFlow.name = newFlowName;
        flowGroup.flows().append(configFlow);
        ui.flow_list_cb->addItem(newFlowName);
        ui.flow_list_cb->setCurrentIndex(ui.flow_list_cb->count() - 1);
        saveConfigToFile();
        updateFlowListWidth();
    }
}

void App::on_btn_flow_edit_clicked() {
    auto currentIndex = ui.flow_list_cb->currentIndex();
    if (currentIndex == -1) {
        return;
    }
    auto newFlowName = FlowNameEditDlg::showDialog(flowGroup.flows()[currentIndex].name(), tr("重命名流程"), this);
    if (!newFlowName.isEmpty()) {
        flowGroup.flows()[currentIndex].name = newFlowName;
        saveConfigToFile();
        ui.flow_list_cb->setItemText(currentIndex, newFlowName);
        updateFlowListWidth();
    }
}

void App::on_btn_flow_remove_clicked() {
    auto currentIndex = ui.flow_list_cb->currentIndex();
    if (currentIndex == -1) {
        return;
    }
    auto dlg = MessageDlg::showMessage(tr("警告"), tr("确定删除流程‘%1’？").arg(flowGroup.flows()[currentIndex].name()), this, true);
    connect(dlg, &MessageDlg::accepted, this, [&, currentIndex] {
        flowGroup.flows().removeAt(currentIndex);
        ui.flow_list_cb->removeItem(currentIndex);
        saveConfigToFile();
        updateFlowListWidth();
    });
}

void App::on_graphic_view_configChanged() {
    saveConfigToFile();
}

void App::on_btn_monitor_clicked() {
    if (monitorDlg) {
        monitorDlg->show();
        return;
    }
    auto currentIndex = ui.flow_list_cb->currentIndex();
    monitorDlg = new MonitorDlg(&remoteControl, currentIndex == -1 ? nullptr : &flowGroup.flows()[currentIndex], this);
    monitorDlg->show();
    connect(monitorDlg, &MonitorDlg::requestSelectNode, this, [&] (const QString& flowName, const QString& uuid) {
        ui.graphic_view->makeStateRunning(flowName, uuid);
    });
}

void App::saveLastOpenFilePathRecord(const QString &filePath) {
    AppSettings::lastOpenFilePath = filePath;
    auto& recentFiles = AppSettings::recentFiles();
    if (recentFiles.contains(filePath)) {
        recentFiles.removeOne(filePath);
    }
    if (recentFiles.size() > 20) {
        recentFiles.removeLast();
    }
    recentFiles.prepend(filePath);
    AppSettings::recentFiles.save();
}

