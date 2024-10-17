#pragma once

#include "../basepropertyeditdlg.h"

#include "ui_multieventstatepropex.h"

#include "data/multieventstatepropdata.h"

class MultiEventStatePropEditDlg : public BasePropertyEditDlg {
    Q_OBJECT

public:
    explicit MultiEventStatePropEditDlg(QWidget *parent = nullptr);

    void setExData(const MultiEventStatePropertyData &data);

    MultiEventStatePropertyData getExEditData() const;

protected:
    void on_btn_confirm_clicked() override;

private:
    Ui::MultiEventStatePropEx exUi;
    MultiEventStatePropertyData exData;

private:
    void addEventEditItem(const EventTriggerFunction& data);
};
