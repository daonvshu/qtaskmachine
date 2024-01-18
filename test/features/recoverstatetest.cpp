#include "recoverstatetest.h"

int RecoverStateTest::id = qRegisterMetaType<RecoverStateTest*>();
RecoverStateTest::RecoverStateTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString RecoverStateTest::getDescription() {
    return u8"使用QHistoryState保存历史状态，历史状态是用于保存一个状态组，当一个子状态发生某种异常切换到一个单独状态时，"
           "从这个单独的状态再切换到历史状态将重新执行上一个子状态。历史状态可以设置是否保存所有的嵌套子状态，默认情况下，"
           "只会保存当前状态组的第一层流程。";
}

QString RecoverStateTest::getCodeFile() {
    return QLatin1String(FEATURES_ROOT_DIR) + "/recoverstaterun.h";
}

void RecoverStateTest::run() {
    if (stateMachine && stateMachine->isRunning()) {
        return;
    }
    delete stateMachine;
    delete eventTrigger;
    eventTrigger = new RecoverEventTrigger(this);
    stateMachine = RecoverStateRun::run(eventTrigger);
}

QWidget *RecoverStateTest::getExtraInputWidgets() {
    auto widget = new QWidget;
    ui.setupUi(widget);
    connect(ui.btn_trigger, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->trigger();
        }
    });
    connect(ui.btn_recover_main, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->recoverMain();
        }
    });
    connect(ui.btn_recover_child, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->recoverChild();
        }
    });
    return widget;
}
