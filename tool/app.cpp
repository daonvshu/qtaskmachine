#include "app.h"

#include "widgets/flowlistitemdelegate.h"

#include <qfiledialog.h>
#include <qstandardpaths.h>

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
    saveFlowConfig(items);
}






