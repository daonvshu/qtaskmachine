#include "commonpropmanager.h"

#include "../ui_app.h"

CommonPropManager::CommonPropManager(Ui::App* appUi, QObject *parent)
    : QObject(parent)
    , appUi(appUi)
{
    connect(appUi->input_name, &QLineEdit::editingFinished, this, &CommonPropManager::nameEditFinished);
    connect(appUi->input_func_enter, &QLineEdit::editingFinished, this, &CommonPropManager::funcEnterEditFinished);
    connect(appUi->input_func_exit, &QLineEdit::editingFinished, this, &CommonPropManager::funcExitEditFinished);
}

void CommonPropManager::loadItemBaseData(FcExecutorItem *item) {
    appUi->input_name->setText(item->itemData.text);
    appUi->input_func_enter->setText(item->itemData.functionEnter);
    appUi->input_func_exit->setText(item->itemData.functionExit);
    activeItem = item;
}

void CommonPropManager::nameEditFinished() {
    if (activeItem) {
        activeItem->itemData.text = appUi->input_name->text();
    }
}

void CommonPropManager::funcEnterEditFinished() {
    if (activeItem) {
        activeItem->itemData.functionEnter = appUi->input_func_enter->text();
    }
}

void CommonPropManager::funcExitEditFinished() {
    if (activeItem) {
        activeItem->itemData.functionExit = appUi->input_func_exit->text();
    }
}
