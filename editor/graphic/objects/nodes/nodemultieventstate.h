#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "../../render/nodes/nodemultieventstaterender.h"
#include "nodemultieventstate.d.h"

class NodeMultiEventState : public GraphicNode {
public:
    explicit NodeMultiEventState(const QSharedPointer<NodeMultiEventStateData>& data);

    static QSharedPointer<NodeMultiEventState> create() {
        return objectCreate<NodeMultiEventState, NodeMultiEventStateData>();
    }

    QSharedPointer<GraphicObject> clone() override {
        return objectCreate<NodeMultiEventState, NodeMultiEventStateData>(*eventStateData);
    }

    GraphicRenderInterface * getRender() override {
        return new NodeMultiEventStateRender(eventStateData);
    }

    GraphicObjectType objectType() override {
        return GraphicObjectType::Node_MultiEvent_State;
    }

public:
    QSharedPointer<NodeMultiEventStateData> eventStateData;
};
