#include "countdowntest.h"

int CountDownTest::id = qRegisterMetaType<CountDownTest*>();
CountDownTest::CountDownTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString CountDownTest::getDescription() {
    return u8"倒计时状态，基于事件触发状态，通过定时器触发检查当前是否到达指定秒数，到达时通过自身信号触发状态切换。";
}

QString CountDownTest::getCodeFile() {
    return QLatin1String(SCENE_ROOT_DIR) + "/countdownrun.h";
}

void CountDownTest::run() {
    if (stateMachine && stateMachine->isRunning()) {
        return;
    }
    delete stateMachine;
    delete eventTrigger;

    eventTrigger = new CountDownEventTrigger(this);
    stateMachine = CountDownRun::run(eventTrigger, ui.input_from->text().toInt(), ui.input_to->text().toInt());
}

QWidget *CountDownTest::getExtraInputWidgets() {
    auto widget = new QWidget;
    ui.setupUi(widget);
    connect(ui.btn_pause, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->pause();
        }
    });
    connect(ui.btn_resume, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->resume();
        }
    });
    connect(ui.btn_skip, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->skip();
        }
    });
    return widget;
}
