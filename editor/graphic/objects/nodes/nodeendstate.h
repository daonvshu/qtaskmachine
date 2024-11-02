#pragma once

#include <qobject.h>

#include "../graphicnode.h"

class NodeEndState : public GraphicNode {
public:
    explicit NodeEndState(const QSharedPointer<GraphicNodeData>& data);

    static QSharedPointer<NodeEndState> create() {
        return objectCreate<NodeEndState, GraphicNodeData>();
    }

    QSharedPointer<GraphicObject> clone() override {
        return objectCreate<NodeEndState, GraphicNodeData>(*nodeData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_End_State;
    }

    void drawObject() override;
};
