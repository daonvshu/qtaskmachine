#include "groupstatepropeditdlg.h"

GroupStatePropEditDlg::GroupStatePropEditDlg(QWidget *parent)
    : BasePropertyEditDlg(parent)
{
    ui.title->setText(tr("状态组属性设置"));

    auto exWidget = new QWidget(this);
    exUi.setupUi(exWidget);
    ui.prop_layout->insertWidget(2, exWidget);

    registerMessageHint(exUi.input_error_trigger, tr("设置异常信号触发函数，所有子状态会继承该触发函数！"));
}

void GroupStatePropEditDlg::setExData(const StateGroupPropertyData &data) {
    exData = data;
    exUi.input_error_trigger->setText(exData.errorTriggerFunc());
}

StateGroupPropertyData GroupStatePropEditDlg::getExEditData() const {
    return exData;
}

void GroupStatePropEditDlg::on_btn_confirm_clicked() {
    exData.errorTriggerFunc = exUi.input_error_trigger->text();
    BasePropertyEditDlg::on_btn_confirm_clicked();
}
