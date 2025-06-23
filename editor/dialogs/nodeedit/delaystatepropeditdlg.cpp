#include "delaystatepropeditdlg.h"

DelayStatePropEditDlg::DelayStatePropEditDlg(QWidget *parent)
    : BasePropertyEditDlg(parent)
{
    ui.title->setText(tr("延时状态属性设置"));

    auto exWidget = new QWidget(this);
    exUi.setupUi(exWidget);
    ui.prop_layout->insertWidget(2, exWidget);

    registerMessageHint(exUi.spinBox, tr("设置延时时间（ms）"));
    registerMessageHint(exUi.input_delay_property, tr("设置读取延时的属性变量，不为空时优先使用该值，属性的类型必须是int。"));
}

void DelayStatePropEditDlg::setExData(const DelayStatePropertyData &data) {
    exData = data;
    exUi.spinBox->setValue(exData.delayMs());
    exUi.input_delay_property->setText(exData.delayProperty());
}

DelayStatePropertyData DelayStatePropEditDlg::getExEditData() const {
    return exData;
}

void DelayStatePropEditDlg::on_btn_confirm_clicked() {
    exData.delayMs = exUi.spinBox->value();
    exData.delayProperty = exUi.input_delay_property->text();
    BasePropertyEditDlg::on_btn_confirm_clicked();
}
