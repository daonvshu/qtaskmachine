#include "loopstatepropeditdlg.h"

LoopStatePropEditDlg::LoopStatePropEditDlg(QWidget* parent)
    : BasePropertyEditDlg(parent)
{
    ui.title->setText(tr("循环执行状态属性编辑"));
    ui.call_on_non_block->setEnabled(true);

    auto exWidget = new QWidget(this);
    exUi.setupUi(exWidget);
    ui.prop_layout->insertWidget(2, exWidget);

    registerMessageHint(exUi.input_total, tr("设置循环总次数，循环到总次数时切换到退出分支\n"
                                             "如果为-1则表示无限循环\n"
                                             "如果是字符串，则当做属性名读取总次数值，属性必须是int类型"));

    registerMessageHint(exUi.input_exit_fun, tr("设置循环退出检查函数，如果被设置，则每次循环执行时调用该函数\n"
                                                "检查函数返回true则退出循环，返回false则继续执行\n"
                                                "函数签名：Q_INVOKABLE bool function(int)"));
}

void LoopStatePropEditDlg::setExData(const LoopStatePropertyData& data) {
    exData = data;
    exUi.input_total->setText(data.total());
    exUi.input_exit_fun->setText(data.exitFunc());
}

LoopStatePropertyData LoopStatePropEditDlg::getExEditData() const {
    return exData;
}

void LoopStatePropEditDlg::on_btn_confirm_clicked() {
    exData.total = exUi.input_total->text();
    exData.exitFunc = exUi.input_exit_fun->text();

    BasePropertyEditDlg::on_btn_confirm_clicked();
}
