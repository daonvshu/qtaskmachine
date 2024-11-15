#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "nodeconditionstate.d.h"

class NodeConditionState : public GraphicNode {
public:
    explicit NodeConditionState(const QSharedPointer<NodeConditionStateData>& data);

    static NodeConditionState* create() {
        return objectCreate<NodeConditionState, NodeConditionStateData>();
    }

    GraphicObject* clone() const override {
        return objectCreate<NodeConditionState, NodeConditionStateData>(*conditionStateData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Condition_State;
    }

    ConfigFlowExecutor toFlowExecutor() const override;

    void fromExecutor(const ConfigFlowExecutor &executor) const override;

    void drawObject() override;

public:
    QSharedPointer<NodeConditionStateData> conditionStateData;
};
