#include "nodemoveaction.h"

NodeMoveAction::NodeMoveAction(const QSharedPointer<NodeMoveActionData> &data)
    : GraphicObject(data)
    , actionData(data)
{
    actionData->oldRenderPosition = data->target->data->oldRenderPosition;
    actionData->renderPosition = data->target->data->renderPosition;
}

void NodeMoveAction::undo() {
    actionData->target->data->renderPosition = actionData->oldRenderPosition;
    actionData->target->data->isChanged = true;
}

void NodeMoveAction::redo() {
    actionData->target->data->renderPosition = actionData->renderPosition;
    actionData->target->data->isChanged = true;
}
