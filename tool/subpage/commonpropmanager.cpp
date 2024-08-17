#include "commonpropmanager.h"

#include "../ui_app.h"

CommonPropManager::CommonPropManager(Ui::App* appUi, QObject *parent)
    : QObject(parent)
    , appUi(appUi)
{
    connect(appUi->input_name, &QLineEdit::editingFinished, this, &CommonPropManager::nameEditFinished);
    connect(appUi->input_func_enter, &QLineEdit::editingFinished, this, &CommonPropManager::funcEnterEditFinished);
    connect(appUi->input_func_exit, &QLineEdit::editingFinished, this, &CommonPropManager::funcExitEditFinished);
    connect(appUi->btn_property_add, &QPushButton::clicked, this, &CommonPropManager::addPropertyBind);
}

void CommonPropManager::loadItemBaseData(FcExecutorItem *item) {
    appUi->input_name->setText(item->itemData.text);
    appUi->input_func_enter->setText(item->itemData.functionEnter);
    appUi->input_func_exit->setText(item->itemData.functionExit);
    loadPropertyBindList(item->itemData.properties);
    activeItem = item;
    activeConditionalItem = nullptr;
}

void CommonPropManager::loadItemBaseData(FcConditionalItem *item) {
    appUi->input_name->setText(item->conditionData.text);
    appUi->input_func_enter->setText(item->conditionData.functionEnter);
    appUi->input_func_exit->setText(item->conditionData.functionExit);
    loadPropertyBindList(item->conditionData.properties);
    activeItem = nullptr;
    activeConditionalItem = item;
}

void CommonPropManager::loadPropertyBindList(QList<PropertyBindData> &data) {
    while (appUi->layout_bind_properties->count() > 1) {
        auto item = appUi->layout_bind_properties->takeAt(0);
        delete item->widget();
        delete item;
    }

    int index = 0;
    for (auto& d : data) {
        auto propertyBindWidget = new PropertyBindWidget(&d, appUi->state_common_prop);
        appUi->layout_bind_properties->insertWidget(index++, propertyBindWidget);
    }
}

void CommonPropManager::nameEditFinished() {
    if (activeItem) {
        activeItem->itemData.text = appUi->input_name->text();
    }
    if (activeConditionalItem) {
        activeConditionalItem->conditionData.text = appUi->input_name->text();
    }
    emit nameChanged();
}

void CommonPropManager::funcEnterEditFinished() {
    if (activeItem) {
        activeItem->itemData.functionEnter = appUi->input_func_enter->text();
    }
    if (activeConditionalItem) {
        activeConditionalItem->conditionData.functionEnter = appUi->input_func_enter->text();
    }
}

void CommonPropManager::funcExitEditFinished() {
    if (activeItem) {
        activeItem->itemData.functionExit = appUi->input_func_exit->text();
    }
    if (activeConditionalItem) {
        activeConditionalItem->conditionData.functionExit = appUi->input_func_exit->text();
    }
}

void CommonPropManager::addPropertyBind() {
    PropertyBindData* bindData = nullptr;
    if (activeItem) {
        activeItem->itemData.properties << PropertyBindData();
        bindData = &activeItem->itemData.properties.last();
    } else if (activeConditionalItem) {
        activeConditionalItem->conditionData.properties << PropertyBindData();
        bindData = &activeConditionalItem->conditionData.properties.last();
    }
    if (bindData == nullptr) {
        return;
    }
    auto propertyBindWidget = new PropertyBindWidget(bindData, appUi->state_common_prop);
    appUi->layout_bind_properties->insertWidget(appUi->layout_bind_properties->count() - 1, propertyBindWidget);
}


PropertyBindWidget::PropertyBindWidget(PropertyBindData* bindData, QWidget *parent)
    : QWidget(parent)
    , bindData(bindData)
{
    ui.setupUi(this);

    ui.cb_call_time->setCurrentIndex(bindData->callOnEntered ? 0 : 1);
    ui.cb_value_type->setCurrentText(bindData->valueType);
    ui.input_key->setText(bindData->key);
    ui.input_value->setText(bindData->value);
}

void PropertyBindWidget::on_cb_call_time_currentIndexChanged(int index) {
    bindData->callOnEntered = index == 0;
}

void PropertyBindWidget::on_cb_value_type_currentTextChanged(const QString &typeName) {
    bindData->valueType = typeName;
}

void PropertyBindWidget::on_input_key_editingFinished() {
    bindData->key = ui.input_key->text();
}

void PropertyBindWidget::on_input_value_editingFinished() {
    bindData->value = ui.input_value->text();
}
