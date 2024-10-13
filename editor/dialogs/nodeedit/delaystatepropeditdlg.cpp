#include "delaystatepropeditdlg.h"

DelayStatePropEditDlg::DelayStatePropEditDlg(QWidget *parent)
    : BasePropertyEditDlg(parent)
{
    ui.title->setText(tr("延时状态属性设置"));

    auto exWidget = new QWidget(this);
    exUi.setupUi(exWidget);
    ui.prop_layout->insertWidget(2, exWidget);

    registerMessageHint(exUi.spinBox, tr("设置延时时间（ms）"));
}

void DelayStatePropEditDlg::setExData(const DelayStatePropertyData &data) {
    exData = data;
    exUi.spinBox->setValue(exData.delayMs());
}

DelayStatePropertyData DelayStatePropEditDlg::getExEditData() const {
    return exData;
}

void DelayStatePropEditDlg::on_btn_confirm_clicked() {
    exData.delayMs = exUi.spinBox->value();
    BasePropertyEditDlg::on_btn_confirm_clicked();
}
