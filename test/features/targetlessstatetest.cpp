#include "targetlessstatetest.h"

int TargetLessStateTest::id = qRegisterMetaType<TargetLessStateTest*>();
TargetLessStateTest::TargetLessStateTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString TargetLessStateTest::getDescription() {
    return u8"未设置状态切换目标时，信号触发或子状态组完成时将重新进入自身，仅DelayState、EventState、GroupState支持。";
}

QString TargetLessStateTest::getCodeFile() {
    return FEATURES_ROOT_DIR + QString("/targetlessstaterun.h");
}

void TargetLessStateTest::run() {
    if (stateMachine && stateMachine->isRunning()) {
        return;
    }
    delete stateMachine;
    delete eventTrigger;
    eventTrigger = new TargetLessEventTrigger(this);
    if (ui.btn_delay->isChecked()) {
        eventTrigger->targetLessType = TargetLessType::Type_Delay;
    } else if (ui.btn_event->isChecked()) {
        eventTrigger->targetLessType = TargetLessType::Type_Event;
    } else if (ui.btn_subgroup->isChecked()) {
        eventTrigger->targetLessType = TargetLessType::Type_SubGroup;
    }
    stateMachine = TargetLessStateRun::run(eventTrigger);
}

QWidget *TargetLessStateTest::getExtraInputWidgets() {
    auto widget = new QWidget;
    ui.setupUi(widget);
    connect(ui.btn_trigger, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->trigger();
        }
    });
    connect(ui.btn_exit, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->exit();
        }
    });
    return widget;
}
