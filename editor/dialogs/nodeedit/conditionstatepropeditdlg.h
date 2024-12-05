#pragma once

#include "../basepropertyeditdlg.h"

#include "ui_conditionstatepropex.h"
#include "ui_conditionbranchitem.h"

#include "data/conditionstatepropdata.h"

class ConditionStatePropEditDlg : public BasePropertyEditDlg {
    Q_OBJECT

public:
    explicit ConditionStatePropEditDlg(QWidget *parent = nullptr);

    void setExData(const ConditionStatePropertyData &data);

    ConditionStatePropertyData getExEditData() const;

protected:
    void on_btn_confirm_clicked() override;

private:
    Ui::ConditionStatePropEx exUi;
    ConditionStatePropertyData exData;

private:
    void addBranchEditItem(int branchId, const QString& name);
};

class ConditionBranchItemWidget : public QWidget {
    Q_OBJECT

public:
    ConditionBranchItemWidget(int id, const QString& name, QWidget* parent = nullptr);

    int getId() const;

    QString getName() const;

signals:
    void removeRequest();

private:
    Ui::ConditionBranchItem ui;
};