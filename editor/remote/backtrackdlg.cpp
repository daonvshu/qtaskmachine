#include "backtrackdlg.h"

#include <QWKWidgets/widgetwindowagent.h>

BackTrackDlg::BackTrackDlg(const QList<ReceiveActiveNode>& activeNodes, ConfigFlow* currentFlow, QWidget *parent)
    : QDialog(parent)
    , activeNodes(activeNodes)
    , currentFlow(currentFlow)
{
    setWindowFlag(Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_DeleteOnClose);

    ui.setupUi(this);

    auto agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);
    agent->setTitleBar(ui.app_title);
    agent->setHitTestVisible(ui.system_buttons);

    loadActiveNodes();

    autoRunTimer = new QTimer(this);
    connect(autoRunTimer, &QTimer::timeout, this, &BackTrackDlg::autoRunMove);
}

void BackTrackDlg::closeEvent(QCloseEvent* event) {
    QDialog::closeEvent(event);
    emit closed();
}

void BackTrackDlg::on_btn_close_clicked() {
    reject();
}

void BackTrackDlg::on_btn_confirm_clicked() {
    accept();
}

void BackTrackDlg::on_btn_begin_pos_clicked() {
    if (activeNodes.isEmpty()) {
        return;
    }
    if (autoRunTimer->isActive()) {
        autoRunTimer->stop();
    }
    clearPos();
    nodePos = 0;
    setPos();
    emit activeNodeSelected(activeNodes[nodePos].nodeId());
}

void BackTrackDlg::on_btn_next_clicked() {
    if (autoRunTimer->isActive()) {
        autoRunTimer->stop();
    }
    if (nodePos + 1 < activeNodes.size()) {
        auto oldPos = nodePos;
        clearPos();
        nodePos++;
        setPos();
        emit activeNodeSelected(activeNodes[nodePos].nodeId());
    }
}

void BackTrackDlg::on_btn_previous_clicked() {
    if (autoRunTimer->isActive()) {
        autoRunTimer->stop();
    }
    if (nodePos - 1 >= 0) {
        clearPos();
        nodePos--;
        setPos();
        emit activeNodeSelected(activeNodes[nodePos].nodeId());
    }
}

void BackTrackDlg::on_btn_click_pos_clicked() {
    if (autoRunTimer->isActive()) {
        autoRunTimer->stop();
    }
    auto item = ui.state_list->currentIndex();
    if (!item.isValid()) {
        return;
    }
    clearPos();
    nodePos = item.row();
    setPos();
    emit activeNodeSelected(activeNodes[nodePos].nodeId());
}

void BackTrackDlg::on_btn_auto_run_clicked() {
    if (autoRunTimer->isActive()) {
        autoRunTimer->stop();
        return;
    }
    clearPos();
    nodePos = -1;
    autoRunTimer->setSingleShot(ui.cb_use_real_interval->isChecked());
    autoRunTimer->start(qMax(1, ui.input_auto_interval->text().toInt()));
}

void BackTrackDlg::loadActiveNodes() {
    ui.state_list->setModel(tbModel = new QStandardItemModel(this));
    const QStringList headers = { "位置", "时间点", "至上次间隔", "状态名" };
    tbModel->setHorizontalHeaderLabels(headers);
    ui.state_list->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui.state_list->horizontalHeader()->setStretchLastSection(true);
    ui.state_list->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QHash<QString, QString> nodeNameMap;
    if (currentFlow != nullptr) {
        for (const auto& node : currentFlow->executors()) {
            nodeNameMap.insert(node.uuid(), node.text());
        }
    }
    qint64 lastTime = -1;
    for (const auto& node : activeNodes) {
        QList<QStandardItem*> items;
        QStandardItem* item;

        //pos col
        item = new QStandardItem;
        items << item;

        //time col
        item = new QStandardItem(QDateTime::fromMSecsSinceEpoch(node.timestamp()).toString("HH:mm:ss.zzz"));
        items << item;

        //interval col
        if (lastTime == -1) {
            lastTime = node.timestamp();
        }
        item = new QStandardItem(QString::number(node.timestamp() - lastTime));
        items << item;
        lastTime = node.timestamp();

        //name col
        item = new QStandardItem(nodeNameMap[node.nodeId()]);
        items << item;

        tbModel->appendRow(items);
    }
}

void BackTrackDlg::clearPos() {
    if (nodePos != -1) {
        auto item = tbModel->index(nodePos, 0);
        tbModel->itemFromIndex(item)->setIcon(QIcon());
    }
}

void BackTrackDlg::setPos() {
    if (nodePos == -1) {
        return;
    }
    auto item = tbModel->index(nodePos, 0);
    ui.state_list->setCurrentIndex(item);
    tbModel->itemFromIndex(item)->setIcon(QIcon(":/res/arrow_right.svg"));
}

void BackTrackDlg::autoRunMove() {
    if (nodePos + 1 < activeNodes.size()) {
        clearPos();
        nodePos++;
        setPos();
        emit activeNodeSelected(activeNodes[nodePos].nodeId());
        if (autoRunTimer->isSingleShot()) {
            if (nodePos + 1 < activeNodes.size()) {
                int nextInterval = tbModel->item(nodePos + 1, 2)->data(Qt::DisplayRole).toString().toInt();
                autoRunTimer->start(qMax(1, ui.input_auto_interval->text().toInt()) + nextInterval);
            }
        }
    } else {
        autoRunTimer->stop();
    }
}
