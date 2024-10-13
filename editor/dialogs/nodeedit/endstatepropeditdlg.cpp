#include "endstatepropeditdlg.h"

EndStatePropEditDlg::EndStatePropEditDlg(QWidget *parent)
    : BasePropertyEditDlg(parent)
{
    ui.title->setText(tr("结束状态属性设置"));
    hideExitFunctionEditor();
}
