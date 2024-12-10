#pragma once

#include <qdialog.h>
#include <qstandarditemmodel.h>
#include <qtimer.h>

#include "ui_backtrackdlg.h"

#include "remote/protocol.h"
#include "data/configflows.h"

class BackTrackDlg : public QDialog {
    Q_OBJECT

public:
    explicit BackTrackDlg(const QList<ReceiveActiveNode>& activeNodes, ConfigFlow* currentFlow, QWidget *parent = nullptr);

signals:
    void activeNodeSelected(const QString& uuid);
    void closed();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::BackTrackDlg ui;
    QList<ReceiveActiveNode> activeNodes;
    int nodePos = -1;
    QStandardItemModel* tbModel;
    ConfigFlow* currentFlow;
    QTimer* autoRunTimer;

private slots:
    void on_btn_close_clicked();
    void on_btn_confirm_clicked();
    void on_btn_begin_pos_clicked();
    void on_btn_next_clicked();
    void on_btn_previous_clicked();
    void on_btn_click_pos_clicked();
    void on_btn_auto_run_clicked();

private:
    void loadActiveNodes();
    void clearPos();
    void setPos();
    void autoRunMove();
};
