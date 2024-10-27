#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "../../render/nodes/nodeendstaterender.h"

class NodeEndState : public GraphicNode {
public:
    explicit NodeEndState(const QSharedPointer<GraphicNodeData>& data);

    static QSharedPointer<NodeEndState> create() {
        return objectCreate<NodeEndState, GraphicNodeData>();
    }

    QSharedPointer<GraphicObject> clone() override {
        return objectCreate<NodeEndState, GraphicNodeData>(*nodeData);
    }

    GraphicRenderInterface * getRender() override {
        return new NodeEndStateRender(data);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_End_State;
    }
};
