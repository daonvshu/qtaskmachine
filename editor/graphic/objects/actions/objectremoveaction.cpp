#include "objectremoveaction.h"

ObjectRemoveAction::ObjectRemoveAction(const QSharedPointer<ObjectRemoveActionData> &data)
    : GraphicObject(data)
    , actionData(data)
{}

void ObjectRemoveAction::undo() {
    for (const auto& obj : actionData->targets) {
        obj->data->assignRemoved = false;
    }
}

void ObjectRemoveAction::redo() {
    for (const auto& obj : actionData->targets) {
        obj->data->assignRemoved = true;
    }
}
