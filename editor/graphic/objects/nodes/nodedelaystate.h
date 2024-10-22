#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "../../render/nodes/nodedelaystaterender.h"
#include "nodedelaystate.d.h"

class NodeDelayState : public GraphicNode {
public:
    explicit NodeDelayState(const QSharedPointer<NodeDelayStateData>& data);

    static QSharedPointer<NodeDelayState> create() {
        return objectCreate<NodeDelayState, NodeDelayStateData>();
    }

    QSharedPointer<GraphicObject> clone() override {
        return objectCreate<NodeDelayState, NodeDelayStateData>(*delayStateData);
    }

    GraphicRenderInterface * getRender() override {
        return new NodeDelayStateRender(delayStateData);
    }

    GraphicObjectType objectType() override {
        return GraphicObjectType::Node_Delay_State;
    }

public:
    QSharedPointer<NodeDelayStateData> delayStateData;
};
