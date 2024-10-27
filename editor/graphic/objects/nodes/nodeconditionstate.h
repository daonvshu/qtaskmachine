#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "../../render/nodes/nodeconditionstaterender.h"
#include "nodeconditionstate.d.h"

class NodeConditionState : public GraphicNode {
public:
    explicit NodeConditionState(const QSharedPointer<NodeConditionStateData>& data);

    static QSharedPointer<NodeConditionState> create() {
        return objectCreate<NodeConditionState, NodeConditionStateData>();
    }

    QSharedPointer<GraphicObject> clone() override {
        return objectCreate<NodeConditionState, NodeConditionStateData>(*conditionStateData);
    }

    GraphicRenderInterface * getRender() override {
        return new NodeConditionStateRender(conditionStateData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Condition_State;
    }

    ConfigFlowExecutor toFlowExecutor() const override;

    void fromExecutor(const ConfigFlowExecutor &executor) override;

public:
    QSharedPointer<NodeConditionStateData> conditionStateData;
};
