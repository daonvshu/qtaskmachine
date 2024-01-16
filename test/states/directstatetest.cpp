#include "directstatetest.h"

#include "directstaterun.h"

int DirectStateTest::id = qRegisterMetaType<DirectStateTest*>();
DirectStateTest::DirectStateTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString DirectStateTest::getDescription() {
    return u8"直接顺序执行状态机，不会等待任何事件，entered与exited事件会立即顺序触发。";
}

QString DirectStateTest::getCodeFile() {
    return QLatin1String(STATE_ROOT_DIR) + "/directstaterun.h";
}

void DirectStateTest::run() {
    if (stateMachine && stateMachine->isRunning()) {
        return;
    }
    delete stateMachine;
    stateMachine = DirectStateRun::run();
}

QWidget *DirectStateTest::getExtraInputWidgets() {
    return nullptr;
}
