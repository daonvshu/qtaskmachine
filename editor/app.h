#pragma once

#include <qwidget.h>
#include <qpointer.h>

#include "ui_app.h"

#include "data/configflows.h"
#include "remote/remotecontrol.h"
#include "widgets/floatingtoolbar.h"

class App : public QWidget {
    Q_OBJECT

public:
    explicit App(QWidget *parent = nullptr);

private:
    Ui::App ui;
    ConfigFlowGroup flowGroup;
    RemoteControl remoteControl;
    QPointer<class MonitorDlg> monitorDlg;
    QPushButton* menuExpandBtn;
    QLabel* searchNextPopLabel;
    FloatingToolbar* floatingToolbar;

private slots:
    void on_btn_min_clicked();
    void on_btn_max_clicked();
    void on_btn_close_clicked();

    void on_btn_new_config_clicked();
    void on_btn_open_config_clicked();
    void on_btn_add_group_clicked();

    void on_graphic_list_flowClicked(ConfigFlow* flow);
    void on_graphic_list_requestClearView();

    void on_graphic_view_configChanged();
    void on_btn_monitor_clicked();
    void on_btn_menu_hide_clicked();
    void on_input_search_text_textEdited(const QString& text);
    void on_input_search_text_editingFinished();
    void on_btn_next_search_result_clicked();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void refreshConfigPathLabel();
    void createNewConfig(const QString& filePath);
    void openExistConfig(const QString& filePath);
    static void saveLastOpenFilePathRecord(const QString& filePath);

    void bindRemoteState();
};
