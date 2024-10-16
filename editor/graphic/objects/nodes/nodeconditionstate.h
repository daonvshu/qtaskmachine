#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "../../render/nodes/nodeconditionstaterender.h"
#include "nodeconditionstate.d.h"

class NodeConditionState : public GraphicNode {
public:
    explicit NodeConditionState(const QSharedPointer<NodeConditionStateData>& data);

    static QSharedPointer<NodeConditionState> create() {
        return QSharedPointer<NodeConditionState>::create(QSharedPointer<NodeConditionStateData>::create());
    }

    GraphicRenderInterface * getRender() override {
        return new NodeConditionStateRender(conditionStateData);
    }

    GraphicObjectType objectType() override {
        return GraphicObjectType::Node_Condition_State;
    }

public:
    QSharedPointer<NodeConditionStateData> conditionStateData;
};
