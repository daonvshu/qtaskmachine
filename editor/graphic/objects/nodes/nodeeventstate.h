#pragma once

#include <qobject.h>

#include "../graphicnode.h"
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

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Event_State;
    }

    ConfigFlowExecutor toFlowExecutor() const override;

    void fromExecutor(const ConfigFlowExecutor &executor) override;

    void drawObject() override;

public:
    QSharedPointer<NodeEventStateData> eventStateData;
};
