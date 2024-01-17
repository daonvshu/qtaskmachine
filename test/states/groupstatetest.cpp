#include "groupstatetest.h"

#include "groupstaterun.h"

int GroupStateTest::id = qRegisterMetaType<GroupStateTest*>();
GroupStateTest::GroupStateTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString GroupStateTest::getDescription() {
    return u8"可以将一系列状态作为一组执行，子状态的父状态必须时分组状态，子状态进入QFinalState才能退出分组状态。";
}

QString GroupStateTest::getCodeFile() {
    return QLatin1String(STATE_ROOT_DIR) + "/groupstaterun.h";
}

void GroupStateTest::run() {
    if (stateMachine && stateMachine->isRunning()) {
        return;
    }
    delete stateMachine;
    stateMachine = GroupStateRun::run();
}

QWidget *GroupStateTest::getExtraInputWidgets() {
    return nullptr;
}
