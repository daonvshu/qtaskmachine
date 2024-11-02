#pragma once

#include <qobject.h>

#include "../graphicnode.h"
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

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_MultiEvent_State;
    }

    void fromExecutor(const ConfigFlowExecutor &executor) override;

    void drawObject() override;

public:
    QSharedPointer<NodeMultiEventStateData> eventStateData;
};
