#include "flowtest.h"

#include <qloggingcategory.h>
#include <qtimer.h>
#include <qdialog.h>

#include "stateutils/taskmachinesteputil.h"

Q_LOGGING_CATEGORY(flowtest, "flowtest")

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
    TaskMachineStepUtil::stepConfig(TASK_CONFIG_DIR);

    if (!runner.isNull()) {
        runner->deleteLater();
    }

    runner = new TaskMachineRunner(ui.input_flow_name->text(), this);
    runner->setLogging(flowtest);
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

    connect(ui.btn_set_prop, &QPushButton::clicked, this, [&] {
        auto propName = ui.input_prop_name->text();
        auto propValue = ui.input_prop_value->text();
        auto prop = this->property(propName.toLatin1());
        if (prop.isValid()) {
            auto propType = prop.type();
            if (propType == QMetaType::QString) {
                setProperty(propName.toLatin1(), propValue);
            } else if (propType == QMetaType::Int) {
                setProperty(propName.toLatin1(), propValue.toInt());
            } else if (propType == QMetaType::Bool) {
                setProperty(propName.toLatin1(), propValue == "true");
            } else if (propType == QMetaType::Double) {
                setProperty(propName.toLatin1(), propValue.toDouble());
            }
        }
    });

    return widget;
}

void FlowTest::onFinishedCall() {
    qDebug() << "finish called";
}

void FlowTest::showDialog() {
    auto obj = new QObject;
    QMetaObject::invokeMethod(obj, [&] {
        QTimer::singleShot(5000, this, [&] {
            emit testEvent1();
        });

        QDialog dialog;
        dialog.exec();
    }, Qt::QueuedConnection);
}

bool FlowTest::checkExitFunction(int index) {
    return index >= 3;
}
