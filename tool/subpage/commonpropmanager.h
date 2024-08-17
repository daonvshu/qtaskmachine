#pragma once

#include <qwidget.h>
#include <qpointer.h>

#include "flowchart/elements/fcexecutoritem.h"
#include "flowchart/elements/fcbranchitem.h"

#include "ui_propertybindwidget.h"

namespace Ui {
    class App;
}

class CommonPropManager : public QObject {
    Q_OBJECT

public:
    explicit CommonPropManager(Ui::App* appUi, QObject *parent = nullptr);

    void loadItemBaseData(FcExecutorItem* item);
    void loadItemBaseData(FcConditionalItem* item);
    void loadPropertyBindList(QList<PropertyBindData>& data);

signals:
    void nameChanged();

private slots:
    void nameEditFinished();
    void funcEnterEditFinished();
    void funcExitEditFinished();
    void addPropertyBind();

private:
    Ui::App* appUi;
    FcExecutorItem* activeItem = nullptr;
    FcConditionalItem* activeConditionalItem = nullptr;
};

class PropertyBindWidget : public QWidget {
    Q_OBJECT

public:
    explicit PropertyBindWidget(PropertyBindData* bindData, QWidget *parent = nullptr);

private slots:
    void on_cb_call_time_currentIndexChanged(int index);
    void on_cb_value_type_currentTextChanged(const QString &typeName);
    void on_input_key_editingFinished();
    void on_input_value_editingFinished();


private:
    PropertyBindData* bindData;
    Ui::PropertyBindWidget ui;
};