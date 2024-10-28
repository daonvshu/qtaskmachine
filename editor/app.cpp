#include "app.h"

#include <QWKWidgets/widgetwindowagent.h>
#include <qdebug.h>
#include <qtimer.h>
#include <qfiledialog.h>
#include <qstandardpaths.h>
#include <qmimedata.h>
#include <qlistview.h>

#include "dialogs/flownameeditdlg.h"

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

    refreshConfigPathLabel();
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
            openExistConfig(urlList.first().toLocalFile());
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
    auto path = QFileDialog::getSaveFileName(nullptr, tr("创建配置"),
                                             configFilePath.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) : configFilePath, "*.json");
    if (path.isEmpty()) {
        return;
    }
    createNewConfig(path);
}

void App::on_btn_open_config_clicked() {
    auto path = QFileDialog::getOpenFileName(nullptr, tr("打开配置"),
                                             configFilePath.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) : configFilePath, "*.json");
    if (path.isEmpty()) {
        return;
    }
    openExistConfig(path);
}

void App::on_flow_list_cb_currentIndexChanged(int index) {
    if (index == -1) {
        ui.graphic_view->updateFlow(nullptr, 2);
    } else {
        ui.graphic_view->updateFlow(&flowGroup.flows()[index], flowGroup.version());
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
        ui.flow_list_cb->setItemText(currentIndex, newFlowName);
        updateFlowListWidth();
    }
}

void App::on_btn_flow_remove_clicked() {
    auto currentIndex = ui.flow_list_cb->currentIndex();
    if (currentIndex == -1) {
        return;
    }
    flowGroup.flows().removeAt(currentIndex);
    ui.flow_list_cb->removeItem(currentIndex);
    updateFlowListWidth();
}

void App::on_graphic_view_configChanged() {
    saveConfigToFile();
}

