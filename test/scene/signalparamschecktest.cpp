#include "signalparamschecktest.h"

#include "signalparamscheckrun1.h"
#include "signalparamscheckrun2.h"

int SignalParamsCheckTest::id = qRegisterMetaType<SignalParamsCheckTest*>();
SignalParamsCheckTest::SignalParamsCheckTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString SignalParamsCheckTest::getDescription() {
    return u8"继承EventState，重载testFinishBySignalData函数可以通过检查信号参数来判断是否切换状态。";
}

QString SignalParamsCheckTest::getCodeFile() {
    return SCENE_ROOT_DIR + QString("/signalparamscheckrun%1.h").arg(getType() + 1);
}

void SignalParamsCheckTest::run() {
    if (stateMachine && stateMachine->isRunning()) {
        return;
    }
    delete stateMachine;
    delete eventTrigger;

    eventTrigger = new SignalParamsCheckEventTrigger(this);
    switch (getType()) {
        case 0:
            stateMachine = SignalParamsCheckRun1::run(eventTrigger);
            break;
        case 1:
            stateMachine = SignalParamsCheckRun2::run(eventTrigger);
            break;
    }
}

QWidget *SignalParamsCheckTest::getExtraInputWidgets() {
    auto widget = new QWidget;
    ui.setupUi(widget);
    connect(ui.btn_trigger, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->trigger(ui.input_param_value->text().toInt());
        }
    });
    connect(ui.buttonGroup, qOverload<QAbstractButton*>(&QButtonGroup::buttonClicked), this, requestCodeReload);
    return widget;
}

int SignalParamsCheckTest::getType() const {
    int type = -1;
    if (ui.btn_type1->isChecked()) {
        type = 0;
    } else if(ui.btn_type2->isChecked()) {
        type = 1;
    }
    return type;
}
