#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "../../render/nodes/nodebeginstaterender.h"

class NodeBeginState : public GraphicNode {
public:
    explicit NodeBeginState(const QSharedPointer<GraphicNodeData>& data);

    static QSharedPointer<NodeBeginState> create() {
        return objectCreate<NodeBeginState, GraphicNodeData>();
    }

    QSharedPointer<GraphicObject> clone() override {
        return objectCreate<NodeBeginState, GraphicNodeData>(*nodeData);
    }

    GraphicRenderInterface * getRender() override {
        return new NodeBeginStateRender(data);
    }

    GraphicObjectType objectType() override {
        return GraphicObjectType::Node_Begin_State;
    }
};
