#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "nodeeventstate.d.h"

class NodeEventState : public GraphicNode {
public:
    explicit NodeEventState(const QSharedPointer<NodeEventStateData>& data, bool initialData = true);

    static NodeEventState* create() {
        return objectCreate<NodeEventState, NodeEventStateData>();
    }

    GraphicObject* clone() const override {
        return objectClone<NodeEventState, NodeEventStateData>(eventStateData, false);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Event_State;
    }

    ConfigFlowExecutor toFlowExecutor() const override;

    void fromExecutor(const ConfigFlowExecutor &executor) const override;

    void drawObject() override;

public:
    QSharedPointer<NodeEventStateData> eventStateData;
};
