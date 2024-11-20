#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "nodedelaystate.d.h"

class NodeDelayState : public GraphicNode {
public:
    explicit NodeDelayState(const QSharedPointer<NodeDelayStateData>& data, bool initialData = true);

    static NodeDelayState* create() {
        return objectCreate<NodeDelayState, NodeDelayStateData>();
    }

    GraphicObject* clone() const override {
        return objectClone<NodeDelayState, NodeDelayStateData>(delayStateData, false);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Delay_State;
    }

    ConfigFlowExecutor toFlowExecutor() const override;

    void fromExecutor(const ConfigFlowExecutor &executor) const override;

    void drawObject() override;

public:
    QSharedPointer<NodeDelayStateData> delayStateData;
};
