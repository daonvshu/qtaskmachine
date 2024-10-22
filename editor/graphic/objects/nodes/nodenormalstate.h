#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "../../render/nodes/nodenormalstaterender.h"

class NodeNormalState : public GraphicNode {
public:
    explicit NodeNormalState(const QSharedPointer<GraphicNodeData>& data);

    static QSharedPointer<NodeNormalState> create() {
        return objectCreate<NodeNormalState, GraphicNodeData>();
    }

    QSharedPointer<GraphicObject> clone() override {
        return objectCreate<NodeNormalState, GraphicNodeData>(*nodeData);
    }

    GraphicRenderInterface * getRender() override {
        return new NodeNormalStateRender(data);
    }

    GraphicObjectType objectType() override {
        return GraphicObjectType::Node_Normal_State;
    }
};
