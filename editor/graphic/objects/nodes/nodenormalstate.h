#pragma once

#include <qobject.h>

#include "../graphicnode.h"

class NodeNormalState : public GraphicNode {
public:
    explicit NodeNormalState(const QSharedPointer<GraphicNodeData>& data);

    static NodeNormalState* create() {
        return objectCreate<NodeNormalState, GraphicNodeData>();
    }

    GraphicObject* clone() const override {
        return objectCreate<NodeNormalState, GraphicNodeData>(*nodeData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Normal_State;
    }

    void drawObject() override;
};
