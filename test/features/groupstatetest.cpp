#include "groupstatetest.h"

int GroupStateTest::id = qRegisterMetaType<GroupStateTest*>();
GroupStateTest::GroupStateTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString GroupStateTest::getDescription() {
    return u8"可以使用其他基础状态作为状态组，将一系列状态作为一组执行，子状态的父状态必须是分组状态，"
           "并且子状态继承父状态所有状态转移，子状态进入QFinalState才能退出分组状态。";
}

QString GroupStateTest::getCodeFile() {
    return FEATURES_ROOT_DIR + QString("/groupstaterun%1.h").arg(ui.btn_type1->isChecked() ? 1 : 2);
}

void GroupStateTest::run() {
    if (stateMachine && stateMachine->isRunning()) {
        return;
    }
    delete stateMachine;
    delete eventTrigger;

    if (ui.btn_type1->isChecked()) {
        stateMachine = GroupStateRun1::run();
    } else {
        eventTrigger = new GroupEventTrigger(this);
        stateMachine = GroupStateRun2::run(eventTrigger);
    }
}

QWidget *GroupStateTest::getExtraInputWidgets() {
    auto widget = new QWidget;
    ui.setupUi(widget);
    connect(ui.buttonGroup, qOverload<QAbstractButton*>(&QButtonGroup::buttonClicked), this, [&] (QAbstractButton* btn) {
        ui.trigger_box->setEnabled(btn == ui.btn_type2);
        requestCodeReload();
    });

    connect(ui.btn_finish, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->finish();
        }
    });
    connect(ui.btn_fail, &QPushButton::clicked, this, [&] {
        if (eventTrigger) {
            eventTrigger->fail();
        }
    });
    return widget;
}
