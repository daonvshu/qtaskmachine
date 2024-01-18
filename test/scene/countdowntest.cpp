#include "countdowntest.h"

int CountDownTest::id = qRegisterMetaType<CountDownTest*>();
CountDownTest::CountDownTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString CountDownTest::getDescription() {
    return QString();
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
