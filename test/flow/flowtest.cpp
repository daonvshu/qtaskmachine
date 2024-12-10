#include "flowtest.h"

int FlowTest::id = qRegisterMetaType<FlowTest*>();
FlowTest::FlowTest(QObject *parent)
    : PageLoadInterface(parent)
{
}

QString FlowTest::getDescription() {
    return {};
}

QString FlowTest::getCodeFile() {
    return {};
}

void FlowTest::run() {
    if (!runner.isNull()) {
        runner->deleteLater();
    }

    runner = new TaskMachineRunner(ui.input_flow_name->text(), this);
    if (!runner->run(this)) {
        qCritical() << "流程执行失败：" << ui.input_flow_name->text();
        runner->deleteLater();
    }
}

QWidget *FlowTest::getExtraInputWidgets() {
    auto widget = new QWidget;
    ui.setupUi(widget);

    connect(ui.btn_event_trigger, &QPushButton::clicked, this, [&] {
        auto eventName = ui.input_event_name->text();
        if (!eventName.isEmpty()) {
            QMetaObject::invokeMethod(this, eventName.toLatin1(), Qt::QueuedConnection);
        }
    });

    return widget;
}
