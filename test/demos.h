#pragma once

#include <qwidget.h>
#include <qpointer.h>
#include <qstandarditemmodel.h>

#include "ui_demos.h"
#include "pageloadinterface.h"

class Demos : public QWidget {
    Q_OBJECT

public:
    explicit Demos(QWidget *parent = nullptr);

    void appendLog(const QString& log);

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::Demos ui;
    QPointer<PageLoadInterface> innerPage;
    QStandardItemModel* treeModel;

private slots:
    void loadPage(const QModelIndex& index);
    void rendPage();

    static void fitHeight(QTextBrowser* browser);

    void on_btn_run_clicked();
};
