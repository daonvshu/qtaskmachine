#include "monitordlg.h"
#include "backtrackdlg.h"

#include "appsettings.h"

#include <QWKWidgets/widgetwindowagent.h>
#include <qfiledialog.h>

MonitorDlg::MonitorDlg(RemoteControl* remoteControl, ConfigFlow* flow, QWidget *parent)
    : QDialog(parent)
    , remoteControl(remoteControl)
    , currentFlow(flow)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlag(Qt::WindowStaysOnTopHint);

    ui.setupUi(this);

    auto agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);
    agent->setTitleBar(ui.app_title);
    agent->setHitTestVisible(ui.system_buttons);

    if (remoteControl->isConnected()) {
        ui.input_ip->setText(remoteControl->getConnectIp());
        ui.input_port->setText(QString::number(remoteControl->getConnectPort()));
        ui.btn_connect->setText("已连接");
    } else {
        ui.input_ip->setText(AppSettings::lastConnectTarget());
        int lastPort = AppSettings::lastConnectPort();
        if (lastPort > 0) {
            ui.input_port->setText(QString::number(lastPort));
        }
    }

    connect(remoteControl, &RemoteControl::connected, this, [&] {
        ui.btn_connect->setText("已连接");
    });

    connect(remoteControl, &RemoteControl::disconnected, this, [&] {
        ui.btn_connect->setText("连接");
        appendLog(QtMsgType::QtCriticalMsg, "连接已断开");
    });

    connect(remoteControl, &RemoteControl::socketError, this, [&] (const QString& errorMsg) {
        appendLog(QtMsgType::QtCriticalMsg, "连接出现错误：" + errorMsg);
    });

    connect(remoteControl, &RemoteControl::stateRefreshed, this, &MonitorDlg::reloadDeviceLogs);

    connect(remoteControl, &RemoteControl::receiveNewLog, this, [&] (const ReceiveNewLog& newLog) {
        if (currentFlow == nullptr) {
            return;
        }
        if (newLog.flowName() != currentFlow->name()) {
            return;
        }
        appendLog(newLog.log());
    });

    connect(remoteControl, &RemoteControl::receiveFlowFinished, this, [&] (const QString& flowName) {
        if (currentFlow == nullptr) {
            return;
        }
        if (flowName == currentFlow->name()) {
            appendLog(QtMsgType::QtWarningMsg, "状态机运行结束！");
        }
    });

    if (currentFlow != nullptr) {
        reloadDeviceLogs();
    }
}

void MonitorDlg::flowChanged(ConfigFlow *flow) {
    currentFlow = flow;
    reloadDeviceLogs();
}

void MonitorDlg::on_btn_close_clicked() {
    reject();
}

void MonitorDlg::on_btn_confirm_clicked() {
    accept();
}

void MonitorDlg::on_btn_connect_clicked() {
    if (remoteControl->isConnected()) {
        remoteControl->closeConnection();
        return;
    }
    auto ip = ui.input_ip->text();
    auto port = ui.input_port->text().toInt();
    if (ip.isEmpty() || port <= 0) {
        return;
    }
    remoteControl->connectToDevice(ip, port);
    AppSettings::lastConnectTarget = ip;
    AppSettings::lastConnectPort = port;
}

void MonitorDlg::on_btn_backtrack_clicked() {
    if (backTrackDlg) {
        backTrackDlg->show();
        return;
    }
    QList<ReceiveActiveNode> stack;
    QString currentFlowName;
    if (currentFlow != nullptr) {
        currentFlowName = currentFlow->name();
        stack = remoteControl->getActiveNodes(currentFlowName);
    }
    backTrackDlg = new BackTrackDlg(stack, currentFlow);
    connect(backTrackDlg, &BackTrackDlg::activeNodeSelected, this, [&, currentFlowName] (const QString& uuid) {
        emit requestSelectNode(currentFlowName, uuid);
    });
    connect(backTrackDlg, &BackTrackDlg::closed, this, [&, currentFlowName] {
        auto currentState = remoteControl->getFlowState(currentFlowName);
        emit requestSelectNode(currentFlowName, currentState.currentRunId());
    });
    backTrackDlg->show();
}

void MonitorDlg::on_btn_clear_log_clicked() {
    ui.view_log->clear();
}

void MonitorDlg::on_btn_export_log_clicked() {
    auto fileName = QFileDialog::getSaveFileName(this, "导出日志", AppSettings::lastExportLogPath(), "日志文件(*.log)");
    if (fileName.isEmpty()) {
        return;
    }
    AppSettings::lastExportLogPath = fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return;
    }
    auto text = ui.view_log->toPlainText();
    file.write(text.toUtf8());
    file.close();
}

void MonitorDlg::appendLog(const FlowRunLogInfo &logInfo) {
    auto msgType = (QtMsgType)logInfo.msgType();
    appendLog(msgType, logInfo.log(), logInfo.timestamp());
}

void MonitorDlg::appendLog(QtMsgType type, const QString &log, qint64 timestamp) {
    switch (type) {
        case QtDebugMsg:
        case QtInfoMsg:
            ui.view_log->setTextColor(Qt::white);
            break;
        case QtWarningMsg:
            ui.view_log->setTextColor(0xFEF838);
            break;
        case QtCriticalMsg:
        case QtFatalMsg:
            ui.view_log->setTextColor(0xFF2828);
            break;
    }
    auto dateTime = timestamp == -1 ? QDateTime::currentDateTime() : QDateTime::fromMSecsSinceEpoch(timestamp);
    ui.view_log->append(QString("[%1] %2").arg(dateTime.toString("HH:mm:ss.zzz"), log));
}

void MonitorDlg::reloadDeviceLogs() {
    ui.view_log->clear();
    if (currentFlow == nullptr) {
        return;
    }
    auto currentState = remoteControl->getFlowState(currentFlow->name());
    for (const auto& logInfo : currentState.logs()) {
        appendLog(logInfo);
    }
}
