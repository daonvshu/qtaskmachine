#include "eventstatetest.h"

int EventStateTest::id = qRegisterMetaType<EventStateTest*>();
EventStateTest::EventStateTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString EventStateTest::getDescription() {
    return u8"<p>事件触发状态转移，通过绑定到指定对象的信号来触发状态转移，其中几个重要的参数设置：\n</p>"
           "<p><span style='font-weight:bold'>setTimeout：</span>设置超时时间，为设置（0ms）将无限等待信号触发。\n</p>"
           "<p><span style='font-weight:bold'>setRetrySize：</span>设置重试次数，如果不为0，超时或失败将重新进入自身状态并触发stateRetry信号。\n</p>"
           "<p><span style='font-weight:bold'>setFailState：</span>设置超时或失败转移的状态，不设置当超时或失败时无法退出当前状态。\n</p>"
           "<p><span style='font-weight:bold'>triggerSignalFail：</span>可主动调用该函数触发失败。\n</p>";
}

QString EventStateTest::getCodeFile() {
    return QLatin1String(STATE_ROOT_DIR) + "/eventstaterun.h";
}

void EventStateTest::run() {
    if (stateMachine && stateMachine->isRunning()) {
        return;
    }
    delete stateMachine;
    delete eventTrigger;

    eventTrigger = new EventTrigger(this);
    stateMachine = EventStateRun::run(eventTrigger, ui.input_timeout->text().toInt(), ui.input_retry->text().toInt());
}

QWidget *EventStateTest::getExtraInputWidgets() {
    auto widget = new QWidget;
    ui.setupUi(widget);
    connect(ui.btn_trigger_finish, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->finish();
        }
    });
    connect(ui.btn_trigger_fail, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->fail();
        }
    });
    return widget;
}
