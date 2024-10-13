#pragma once

#include <qdialog.h>
#include <qhash.h>

#include "ui_basepropertyeditdlg.h"
#include "ui_propertybindwidget.h"

#include "data/basepropertydata.h"

class BasePropertyEditDlg : public QDialog {
    Q_OBJECT

public:
    explicit BasePropertyEditDlg(QWidget *parent = nullptr);

    void setData(const BasePropertyData& data);

    BasePropertyData getEditData() const;

protected:
    void clearDefault();

    void registerMessageHint(QWidget* widget, const QString& message);

    void hideEnterFunctionEditor();

    void hideExitFunctionEditor();

    bool eventFilter(QObject *obj, QEvent *e) override;

protected slots:
    void on_btn_property_add_clicked();
    void on_btn_close_clicked();
    void on_btn_cancel_clicked();
    virtual void on_btn_confirm_clicked();

protected:
    Ui::BasePropertyEditDlg ui;
    BasePropertyData editData;

    QHash<QObject*, QString> messageHint;
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