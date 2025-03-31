#include "app.h"

#include <QWKWidgets/widgetwindowagent.h>
#include <qtimer.h>
#include <qfiledialog.h>
#include <qstandardpaths.h>
#include <qmimedata.h>
#include <qaction.h>
#include <qmenu.h>

#include "appsettings.h"

#include "dialogs/nameeditdlg.h"
#include "dialogs/messagedlg.h"
#include "remote/monitordlg.h"

App::App(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.splitter->setStretchFactor(1, QSizePolicy::Expanding);
    ui.splitter->setSizes({240, 1});

    menuExpandBtn = new QPushButton(this);
    menuExpandBtn->resize(24, 48);
    menuExpandBtn->setStyleSheet("QPushButton{border:none;background:url(:/res/menu_expand.svg);}"
        "QPushButton:hover{background:url(:/res/menu_expand_hover.svg);}");
    menuExpandBtn->raise();
    menuExpandBtn->setVisible(false);
    connect(menuExpandBtn, &QPushButton::clicked, this, [&] {
        ui.graphic_list_body->setVisible(true);
        menuExpandBtn->setVisible(false);
    });

    ui.graphic_list->setData(&flowGroup);

    auto agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);
    agent->setTitleBar(ui.app_title);
    agent->setHitTestVisible(ui.system_buttons);
    agent->setHitTestVisible(ui.graphic_list);
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
    menuExpandBtn->move(0, ui.graphic_list_body->mapTo(this, QPoint(0, 0)).y() + 1);

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
    auto openFilePath = flowGroup.configFilePath;
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
        auto openFilePath = flowGroup.configFilePath;
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

void App::on_btn_add_group_clicked() {
    ui.graphic_list->createNewGroup();
}

void App::on_graphic_list_flowClicked(ConfigFlow* flow) {
    ui.graphic_view->updateFlow(flow);
    if (monitorDlg) {
        monitorDlg->flowChanged(flow);
    }
}

void App::on_graphic_list_requestClearView() {
    ui.graphic_view->updateFlow(nullptr);
}

void App::on_graphic_view_configChanged() {
    ui.graphic_list->saveConfig();
}

void App::on_btn_monitor_clicked() {
    if (monitorDlg) {
        monitorDlg->show();
        return;
    }
    auto currentFlow = ui.graphic_list->getCurrentSelectedFlow();
    monitorDlg = new MonitorDlg(&remoteControl, currentFlow, this);
    monitorDlg->show();
    connect(monitorDlg, &MonitorDlg::requestSelectNode, this, [&] (const QString& flowName, const QString& uuid) {
        ui.graphic_view->makeStateRunning(flowName, uuid);
    });
}

void App::on_btn_menu_hide_clicked() {
    ui.graphic_list_body->setVisible(false);
    menuExpandBtn->setVisible(true);
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

