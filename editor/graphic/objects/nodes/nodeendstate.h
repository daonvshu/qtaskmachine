#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "../../render/nodes/nodeendstaterender.h"

class NodeEndState : public GraphicNode {
public:
    explicit NodeEndState(const QSharedPointer<GraphicNodeData>& data);

    static QSharedPointer<NodeEndState> create() {
        return QSharedPointer<NodeEndState>::create(QSharedPointer<GraphicNodeData>::create());
    }

    GraphicRenderInterface * getRender() override {
        return new NodeEndStateRender(data);
    }

    GraphicObjectType objectType() override {
        return GraphicObjectType::Node_End_State;
    }
};
