#include "beginstatepropeditdlg.h"

BeginStatePropEditDlg::BeginStatePropEditDlg(QWidget *parent)
    : BasePropertyEditDlg(parent)
{
    ui.title->setText(tr("开始状态属性设置"));
    hideEnterFunctionEditor();
}
