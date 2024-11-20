#include "objectcopyaction.h"

ObjectCopyAction::ObjectCopyAction(const QSharedPointer<ObjectCopyActionData> &data)
    : GraphicObject(data)
    , actionData(data)
{}

void ObjectCopyAction::undo() {
    for (const auto& obj : actionData->targets) {
        obj->data->assignRemoved = true;
    }
}

void ObjectCopyAction::redo() {
    for (const auto& obj : actionData->targets) {
        obj->data->assignRemoved = false;
    }
}