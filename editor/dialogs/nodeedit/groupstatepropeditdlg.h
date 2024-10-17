#pragma once

#include "../basepropertyeditdlg.h"

#include "ui_groupstatepropex.h"

#include "data/stategrouppropdata.h"

class GroupStatePropEditDlg : public BasePropertyEditDlg {
    Q_OBJECT

public:
    explicit GroupStatePropEditDlg(QWidget *parent = nullptr);

    void setExData(const StateGroupPropertyData &data);

    StateGroupPropertyData getExEditData() const;

protected:
    void on_btn_confirm_clicked() override;

private:
    Ui::GroupStatePropEx exUi;
    StateGroupPropertyData exData;
};
