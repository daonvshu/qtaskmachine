#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "../../render/nodes/nodeeventstaterender.h"
#include "nodeeventstate.d.h"

class NodeEventState : public GraphicNode {
public:
    explicit NodeEventState(const QSharedPointer<NodeEventStateData>& data);

    static QSharedPointer<NodeEventState> create() {
        return objectCreate<NodeEventState, NodeEventStateData>();
    }

    QSharedPointer<GraphicObject> clone() override {
        return objectCreate<NodeEventState, NodeEventStateData>(*eventStateData);
    }

    GraphicRenderInterface * getRender() override {
        return new NodeEventStateRender(eventStateData);
    }

    GraphicObjectType objectType() override {
        return GraphicObjectType::Node_Event_State;
    }

public:
    QSharedPointer<NodeEventStateData> eventStateData;
};
