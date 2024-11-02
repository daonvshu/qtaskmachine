#pragma once

#include <qobject.h>

#include "../graphicnode.h"

class NodeBeginState : public GraphicNode {
public:
    explicit NodeBeginState(const QSharedPointer<GraphicNodeData>& data);

    static QSharedPointer<NodeBeginState> create() {
        return objectCreate<NodeBeginState, GraphicNodeData>();
    }

    QSharedPointer<GraphicObject> clone() override {
        return objectCreate<NodeBeginState, GraphicNodeData>(*nodeData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Begin_State;
    }

    void drawObject() override;
};
