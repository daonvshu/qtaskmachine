#pragma once

#include "../basepropertyeditdlg.h"

#include "ui_eventstatepropex.h"

#include "data/eventstatepropdata.h"

class EventStatePropEditDlg : public BasePropertyEditDlg {
    Q_OBJECT

public:
    explicit EventStatePropEditDlg(QWidget *parent = nullptr);

    void setExData(const EventStatePropertyData &data);

    EventStatePropertyData getExEditData() const;

protected:
    void on_btn_confirm_clicked() override;

private:
    Ui::EventStatePropEx exUi;
    EventStatePropertyData exData;
};
