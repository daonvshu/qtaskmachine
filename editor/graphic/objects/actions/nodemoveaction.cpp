#include "nodemoveaction.h"

NodeMoveAction::NodeMoveAction(const QSharedPointer<NodeMoveActionData> &data)
    : GraphicObject(data)
    , actionData(data)
{
    for (const auto& node : data->targets) {
        actionData->oldRenderPositions << node->data->oldRenderPosition;
        actionData->renderPositions << node->data->renderPosition;
    }
}

void NodeMoveAction::undo() {
    for (int i = 0; i < actionData->targets.size(); i++) {
        actionData->targets[i]->data->renderPosition = actionData->oldRenderPositions[i];
        actionData->targets[i]->data->isChanged = true;
    }
}

void NodeMoveAction::redo() {
    for (int i = 0; i < actionData->targets.size(); i++) {
        actionData->targets[i]->data->renderPosition = actionData->renderPositions[i];
        actionData->targets[i]->data->isChanged = true;
    }
}
