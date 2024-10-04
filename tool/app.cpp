#include "app.h"

#include "widgets/flowlistitemdelegate.h"

#include "subpage/configdelaystate.h"
#include "subpage/configeventstate.h"
#include "subpage/configmultieventstate.h"
#include "subpage/configconditionstate.h"
#include "subpage/configgroupstate.h"
#include "subpage/confighistorystate.h"

#include <qfiledialog.h>
#include <qstandardpaths.h>
#include <qmessagebox.h>

App::App(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    auto delegate = new FlowListItemDelegate(this);
    connect(delegate, &FlowListItemDelegate::nameChanged, this, &App::itemNameChanged);
    connect(delegate, &FlowListItemDelegate::clickRemove, this, &App::flowItemRemove);
    ui.flow_list->setItemDelegate(delegate);
    ui.flow_list->setModel(flowsMode = new QStandardItemModel(this));
    ui.flow_list->setResizeMode(QListView::Adjust);

    ui.state_common_prop->setVisible(false);

    connect(ui.flow_view, &FlowChartWidget::nodeSelected, this, &App::nodeSelected);

    refreshConfigPathLabel();

    commonPropManager = new CommonPropManager(&ui, this);
    connect(commonPropManager, &CommonPropManager::nameChanged, ui.flow_view, &FlowChartWidget::updateSelectedNodeGeometry);

    ui.stackedWidget->addWidget(new ConfigDelayState(this));
    ui.stackedWidget->addWidget(new ConfigEventState(this));
    ui.stackedWidget->addWidget(new ConfigMultiEventState(this));
    ui.stackedWidget->addWidget(new ConfigConditionState(this));
    ui.stackedWidget->addWidget(new ConfigGroupState(this));
    ui.stackedWidget->addWidget(new ConfigHistoryState(this));

    for (int i = 0; i < ui.stackedWidget->count(); i++) {
        auto widget = ui.stackedWidget->widget(i);
        if (auto config = dynamic_cast<StateConfigInterface*>(widget)) {
            config->geometryChanged = [this] {
                ui.flow_view->updateSelectedNodeGeometry();
            };
            config->viewChanged = [this] {
                ui.flow_view->repaint();
            };
        }
    }

    flowStateRunner = new FlowStateRunner(this);
}

void App::on_btn_new_config_clicked() {
    auto path = QFileDialog::getSaveFileName(nullptr, QStringLiteral("创建配置"),
                                             configFilePath.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) : configFilePath, "*.json");
    if (path.isEmpty()) {
        return;
    }
    createNewConfig(path);
}

void App::on_btn_open_config_clicked() {
    auto path = QFileDialog::getOpenFileName(nullptr, QStringLiteral("打开配置"),
                                             configFilePath.isEmpty() ? QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) : configFilePath, "*.json");
    if (path.isEmpty()) {
        return;
    }
    openExistConfig(path);
}

void App::on_flow_list_clicked(const QModelIndex &index) {
    reloadFlowConfig(index.row());
}

void App::on_btn_create_new_clicked() {
    ConfigFlow configFlow;
    configFlow.name = QStringLiteral("新流程");
    flowGroup.flows().append(configFlow);

    auto item = new QStandardItem(configFlow.name());
    flowsMode->appendRow(item);
}

void App::on_btn_remove_item_clicked() {
    ui.flow_view->removeSelected();
}

void App::on_btn_save_item_clicked() {
    ui.flow_view->resetFlowsId();
    auto items = ui.flow_view->getAllItems();
    if (saveFlowConfig(items)) {
        QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("OK!"));
    }
}

void App::on_btn_flow_start_clicked() {
    ui.flow_view->resetFlowsId();
    auto items = ui.flow_view->getAllItems();
    if (!saveFlowConfig(items)) {
        return;
    }
    beginCurrentState();
}

void App::on_btn_flow_stop_clicked() {
    cancelCurrentState();
}
