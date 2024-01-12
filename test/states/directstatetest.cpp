#include "directstatetest.h"

#include "directstaterun.h"

int DirectStateTest::id = qRegisterMetaType<DirectStateTest*>();
DirectStateTest::DirectStateTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString DirectStateTest::getDescription() {
    return R"(<span style="font-size:14px;">直接顺序执行状态机)";
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
