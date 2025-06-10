#pragma once

#include "../basepropertyeditdlg.h"

#include "ui_loopstatepropex.h"

#include "data/loopstatepropdata.h"

class LoopStatePropEditDlg : public BasePropertyEditDlg {
    Q_OBJECT

public:
    explicit LoopStatePropEditDlg(QWidget* parent = nullptr);

    void setExData(const LoopStatePropertyData &data);

    LoopStatePropertyData getExEditData() const;

protected:
    void on_btn_confirm_clicked() override;

private:
    Ui::LoopStatePropEx exUi;
    LoopStatePropertyData exData;
};
