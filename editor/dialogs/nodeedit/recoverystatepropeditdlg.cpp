#include "recoverystatepropeditdlg.h"

RecoveryStatePropEditDlg::RecoveryStatePropEditDlg(QWidget *parent)
    : BasePropertyEditDlg(parent)
{
    ui.title->setText(tr("恢复点属性设置"));

    auto exWidget = new QWidget(this);
    exUi.setupUi(exWidget);
    ui.prop_layout->insertWidget(2, exWidget);

    registerMessageHint(exUi.check_nested_point, tr("设置为嵌套状态，能够恢复到子状态组中的子状态"));
}

void RecoveryStatePropEditDlg::setExData(const RecoveryStatePropertyData &data) {
    exData = data;
    exUi.check_nested_point->setChecked(exData.nested());
}

RecoveryStatePropertyData RecoveryStatePropEditDlg::getExEditData() const {
    return exData;
}

void RecoveryStatePropEditDlg::on_btn_confirm_clicked() {
    exData.nested = exUi.check_nested_point->isChecked();
    BasePropertyEditDlg::on_btn_confirm_clicked();
}
