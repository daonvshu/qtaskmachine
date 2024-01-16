#include "delaystatetest.h"

#include "delaystaterun.h"

int DelayStateTest::id = qRegisterMetaType<DelayStateTest*>();
DelayStateTest::DelayStateTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString DelayStateTest::getDescription() {
    return u8"延时执行状态机，用于延时指定时间后切换到下一个状态，可以监听entered和exited信号用来"
           "执行需要等待指定时间的两个步骤。";
}

QString DelayStateTest::getCodeFile() {
    return QLatin1String(STATE_ROOT_DIR) + "/delaystaterun.h";
}

void DelayStateTest::run() {
    if (stateMachine && stateMachine->isRunning()) {
        return;
    }
    delete stateMachine;
    stateMachine = DelayStateRun::run(ui.input_delay->text().toInt());
}

QWidget *DelayStateTest::getExtraInputWidgets() {
    auto widget = new QWidget;
    ui.setupUi(widget);
    return widget;
}
