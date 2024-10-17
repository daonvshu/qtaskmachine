#include "eventtriggerinputwidget.h"

#include <qstyleoption.h>
#include <qpainter.h>

EventTriggerInputWidget::EventTriggerInputWidget(const EventTriggerFunction& triggerFunction, QWidget *parent)
    : QWidget(parent)
    , triggerFunction(triggerFunction)
{
    ui.setupUi(this);

    connect(ui.btn_remove, &QPushButton::clicked, this, &EventTriggerInputWidget::removeRequest);

    ui.input_id->setValue(triggerFunction.branchId());
    ui.input_trigger->setText(triggerFunction.triggerFunc());
    ui.input_check->setText(triggerFunction.checkFunc());
}

void EventTriggerInputWidget::hideIdInput() {
    ui.label_id->setVisible(false);
    ui.input_id->setVisible(false);
}

void EventTriggerInputWidget::hideRemoveButton() {
    ui.btn_remove->setVisible(false);
}

void EventTriggerInputWidget::on_input_id_valueChanged(int value) {
    triggerFunction.branchId = value;
}

void EventTriggerInputWidget::on_input_trigger_editingFinished() {
    triggerFunction.triggerFunc = ui.input_trigger->text();
}

void EventTriggerInputWidget::on_input_check_editingFinished() {
    triggerFunction.checkFunc = ui.input_check->text();
}

QSpinBox *EventTriggerInputWidget::getIdInput() const {
    return ui.input_id;
}

QLineEdit *EventTriggerInputWidget::getTriggerInput() const {
    return ui.input_trigger;
}

QLineEdit *EventTriggerInputWidget::getCheckInput() const {
    return ui.input_check;
}

void EventTriggerInputWidget::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
