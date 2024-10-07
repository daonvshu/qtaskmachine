#include "normalstatepropeditdlg.h"

NormalStatePropEditDlg::NormalStatePropEditDlg(QWidget *parent)
    : BasePropertyEditDlg(parent)
{
    ui.title->setText(tr("普通状态属性设置"));
}
