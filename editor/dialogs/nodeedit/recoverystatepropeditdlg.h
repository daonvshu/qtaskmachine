#pragma once

#include "../basepropertyeditdlg.h"

#include "ui_recoverystatepropex.h"

#include "data/recoverystatepropdata.h"

class RecoveryStatePropEditDlg : public BasePropertyEditDlg {
    Q_OBJECT

public:
    explicit RecoveryStatePropEditDlg(QWidget *parent = nullptr);

    void setExData(const RecoveryStatePropertyData &data);

    RecoveryStatePropertyData getExEditData() const;

protected:
    void on_btn_confirm_clicked() override;

private:
    Ui::RecoveryStatePropEx exUi;
    RecoveryStatePropertyData exData;
};
