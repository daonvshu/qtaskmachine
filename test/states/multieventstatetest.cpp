#include "multieventstatetest.h"

int MultiEventStateTest::id = qRegisterMetaType<MultiEventStateTest*>();
MultiEventStateTest::MultiEventStateTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString MultiEventStateTest::getDescription() {
    return u8"通过绑定多个信号，触发不同的状态转移。添加的信号数量必须和切换的目标状态数量一致。";
}

QString MultiEventStateTest::getCodeFile() {
    return QLatin1String(STATE_ROOT_DIR) + "/multieventstaterun.h";
}

void MultiEventStateTest::run() {
    if (stateMachine && stateMachine->isRunning()) {
        return;
    }
    delete stateMachine;
    delete eventTrigger;

    eventTrigger = new MultiEventTrigger(this);
    stateMachine = MultiEventStateRun::run(eventTrigger);
}

QWidget *MultiEventStateTest::getExtraInputWidgets() {
    auto widget = new QWidget;
    ui.setupUi(widget);
    connect(ui.btn_event_1, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->event1();
        }
    });
    connect(ui.btn_event_2, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->event2();
        }
    });
    connect(ui.btn_event_3, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->event3();
        }
    });
    return widget;
}
