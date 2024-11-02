#pragma once

#include <qobject.h>

#include "../graphicnode.h"

class NodeNormalState : public GraphicNode {
public:
    explicit NodeNormalState(const QSharedPointer<GraphicNodeData>& data);

    static QSharedPointer<NodeNormalState> create() {
        return objectCreate<NodeNormalState, GraphicNodeData>();
    }

    QSharedPointer<GraphicObject> clone() override {
        return objectCreate<NodeNormalState, GraphicNodeData>(*nodeData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Normal_State;
    }

    void drawObject() override;
};
