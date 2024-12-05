#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "nodeconditionstate.d.h"

class NodeConditionState : public GraphicNode {
public:
    explicit NodeConditionState(const QSharedPointer<NodeConditionStateData>& data, bool initialData = true);

    static NodeConditionState* create() {
        return objectCreate<NodeConditionState, NodeConditionStateData>();
    }

    GraphicObject* clone() const override {
        return objectClone<NodeConditionState, NodeConditionStateData>(conditionStateData, false);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Condition_State;
    }

    bool testLinkLineIndexValid(int linkIndex, bool isInputPoint) const override;

    ConfigFlowExecutor toFlowExecutor() const override;

    void fromExecutor(const ConfigFlowExecutor &executor) const override;

    void drawObject() override;

public:
    QSharedPointer<NodeConditionStateData> conditionStateData;
};
