#pragma once

#include <qobject.h>

#include "../graphicnode.h"

class NodeEndState : public GraphicNode {
public:
    explicit NodeEndState(const QSharedPointer<GraphicNodeData>& data);

    static NodeEndState* create() {
        return objectCreate<NodeEndState, GraphicNodeData>();
    }

    GraphicObject* clone() const override {
        return objectCreate<NodeEndState, GraphicNodeData>(*nodeData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_End_State;
    }

    void drawObject() override;
};
