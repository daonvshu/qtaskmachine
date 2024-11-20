#pragma once

#include <qobject.h>

#include "nodemoveaction.d.h"

class NodeMoveAction : public GraphicObject {
public:
    explicit NodeMoveAction(const QSharedPointer<NodeMoveActionData>& data);

    static NodeMoveAction* create(const QList<const GraphicNode*>& targets) {
        return objectCreate<NodeMoveAction, NodeMoveActionData>(targets);
    }

    GraphicObject * clone() const override {
        return objectCreate<NodeMoveAction, NodeMoveActionData>(*actionData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Action_Node_Move;
    }

    void undo() override;

    void redo() override;

public:
    QSharedPointer<NodeMoveActionData> actionData;
};
