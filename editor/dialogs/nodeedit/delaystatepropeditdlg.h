#pragma once

#include "../basepropertyeditdlg.h"

#include "ui_delaystatepropex.h"

#include "data/delaystatepropertydata.h"

class DelayStatePropEditDlg : public BasePropertyEditDlg {
    Q_OBJECT

public:
    explicit DelayStatePropEditDlg(QWidget *parent = nullptr);

    void setExData(const DelayStatePropertyData &data);

    DelayStatePropertyData getExEditData() const;

protected:
    void on_btn_confirm_clicked() override;

private:
    Ui::DelayStatePropEx exUi;
    DelayStatePropertyData exData;
};
