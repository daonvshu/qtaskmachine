#include "basepropertyeditdlg.h"

#include <QWKWidgets/widgetwindowagent.h>
#include <qlistview.h>

BasePropertyEditDlg::BasePropertyEditDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    auto agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);
    agent->setTitleBar(ui.app_title);
    agent->setHitTestVisible(ui.system_buttons);

    clearDefault();

    registerMessageHint(ui.input_name, tr("设置打印时的名称"));
    registerMessageHint(ui.input_func_enter, tr("设置状态进入绑定的槽函数"));
    registerMessageHint(ui.input_func_exit, tr("设置状态退出绑定的槽函数"));
    registerMessageHint(ui.btn_property_add, tr("设置状态进入或退出时应用属性值"));
}

void BasePropertyEditDlg::setData(const BasePropertyData &data) {
    editData = data;
    ui.input_name->setText(data.nodeName());
    ui.input_func_enter->setText(data.funcEnter());
    ui.input_func_exit->setText(data.funcExit());
    ui.print_on_enter->setChecked(data.printOnEnter());
    ui.print_on_exit->setChecked(data.printOnExit());
    ui.call_on_non_block->setChecked(data.callNonBlock());
    if (!data.properties().isEmpty()) {
        for (auto& property : editData.properties()) {
            addPropertyEditItem(property);
        }
    }
}

BasePropertyData BasePropertyEditDlg::getEditData() const {
    return editData;
}

void BasePropertyEditDlg::clearDefault() {
    auto buttons = findChildren<QPushButton*>();
    for (auto button : buttons) {
        button->setDefault(false);
        button->setAutoDefault(false);
    }
}

void BasePropertyEditDlg::registerMessageHint(QWidget *widget, const QString &message) {
    messageHint.insert(widget, message);
    widget->installEventFilter(this);
}

void BasePropertyEditDlg::hideEnterFunctionEditor() {
    ui.label_func_enter->setVisible(false);
    ui.input_func_enter->setVisible(false);
}

void BasePropertyEditDlg::hideExitFunctionEditor() {
    ui.label_func_exit->setVisible(false);
    ui.input_func_exit->setVisible(false);
}

bool BasePropertyEditDlg::eventFilter(QObject *obj, QEvent *e) {
    if (e->type() == QEvent::HoverEnter) {
        if (messageHint.contains(obj)) {
            ui.input_hint_message->setText(messageHint[obj]);
        }
    } else if (e->type() == QEvent::HoverLeave) {
        ui.input_hint_message->clear();
    }
    return QDialog::eventFilter(obj, e);
}

void BasePropertyEditDlg::on_btn_property_add_clicked() {
    addPropertyEditItem();
}

void BasePropertyEditDlg::on_btn_close_clicked() {
    reject();
}

void BasePropertyEditDlg::on_btn_cancel_clicked() {
    reject();
}

void BasePropertyEditDlg::on_btn_confirm_clicked() {
    editData.nodeName() = ui.input_name->text();
    editData.funcEnter() = ui.input_func_enter->text();
    editData.funcExit() = ui.input_func_exit->text();
    editData.printOnEnter() = ui.print_on_enter->isChecked();
    editData.printOnExit() = ui.print_on_exit->isChecked();
    editData.callNonBlock() = ui.call_on_non_block->isChecked();
    editData.properties().clear();
    for (int i = 0; i < ui.layout_bind_properties->count() - 1; ++i) {
        auto widget = ui.layout_bind_properties->itemAt(i)->widget();
        if (auto propertyBindWidget = dynamic_cast<PropertyBindWidget*>(widget)) {
            editData.properties().append(propertyBindWidget->bindData);
        }
    }
    accept();
}

void BasePropertyEditDlg::addPropertyEditItem(const PropertyBindData& bindData) {
    auto propertyBindWidget = new PropertyBindWidget(bindData, ui.state_common_prop);
    ui.layout_bind_properties->insertWidget(ui.layout_bind_properties->count() - 1, propertyBindWidget);
    connect(propertyBindWidget, &PropertyBindWidget::removeRequest, this, [&, propertyBindWidget] {
        ui.layout_bind_properties->removeWidget(propertyBindWidget);
        propertyBindWidget->deleteLater();
    });
}

PropertyBindWidget::PropertyBindWidget(const PropertyBindData& bindData, QWidget *parent)
    : QWidget(parent)
    , bindData(bindData)
{
    ui.setupUi(this);

    ui.cb_call_time->setCurrentIndex(bindData.callOnEntered() ? 0 : 1);
    ui.cb_value_type->setCurrentText(bindData.valueType());
    ui.input_key->setText(bindData.key());
    ui.input_value->setText(bindData.value());
    ui.cb_value_type->setView(new QListView);

    connect(ui.btn_remove, &QPushButton::clicked, this, &PropertyBindWidget::removeRequest);
}

void PropertyBindWidget::on_cb_call_time_currentIndexChanged(int index) {
    bindData.callOnEntered = index == 0;
}

void PropertyBindWidget::on_cb_value_type_currentTextChanged(const QString &typeName) {
    bindData.valueType = typeName;
}

void PropertyBindWidget::on_input_key_editingFinished() {
    bindData.key = ui.input_key->text();
}

void PropertyBindWidget::on_input_value_editingFinished() {
    bindData.value = ui.input_value->text();
}