#include "objectremoveaction.h"

ObjectRemoveAction::ObjectRemoveAction(const QSharedPointer<ObjectRemoveActionData> &data)
    : GraphicObject(data)
    , actionData(data)
{}

void ObjectRemoveAction::undo() {
    actionData->target->data->assignRemoved = false;
}

void ObjectRemoveAction::redo() {
    actionData->target->data->assignRemoved = true;
}
