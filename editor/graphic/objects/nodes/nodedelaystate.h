#pragma once

#include <qobject.h>

#include "../graphicnode.h"
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

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Delay_State;
    }

    ConfigFlowExecutor toFlowExecutor() const override;

    void fromExecutor(const ConfigFlowExecutor &executor) override;

    void drawObject() override;

public:
    QSharedPointer<NodeDelayStateData> delayStateData;
};
