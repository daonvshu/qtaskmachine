#pragma once

#include "../basepropertyeditdlg.h"

#include "ui_conditionstatepropex.h"

#include "data/conditionstatepropdata.h"
#include "widget/flowlayout.h"

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
    FlowLayout* branchLayout;

private:
    void addBranchEditItem(int branchId);
};
