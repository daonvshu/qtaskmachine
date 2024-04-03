#include "debugstatetest.h"

Q_LOGGING_CATEGORY(debugState, "state.debug_test")

int DebugStateTest::id = qRegisterMetaType<DebugStateTest*>();
DebugStateTest::DebugStateTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString DebugStateTest::getDescription() {
    return u8"设置状态名称标志和LoggingCategory，在状态进入和离开时打印信息。";
}

QString DebugStateTest::getCodeFile() {
    return FEATURES_ROOT_DIR + QString("/debugstaterun.h");
}

void DebugStateTest::run() {
    if (stateMachine && stateMachine->isRunning()) {
        return;
    }
    delete stateMachine;
    stateMachine = DebugStateRun::run();
}

QWidget *DebugStateTest::getExtraInputWidgets() {
    return nullptr;
}
