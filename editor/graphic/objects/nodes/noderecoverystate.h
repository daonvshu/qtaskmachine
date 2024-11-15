#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "noderecoverystate.d.h"

class NodeRecoveryState : public GraphicNode {
public:
    explicit NodeRecoveryState(const QSharedPointer<NodeRecoveryStateData>& data);

    static NodeRecoveryState* create() {
        return objectCreate<NodeRecoveryState, NodeRecoveryStateData>();
    }

    GraphicObject* clone() const override {
        return objectCreate<NodeRecoveryState, NodeRecoveryStateData>(*recoveryStateData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Recovery_State;
    }

    ConfigFlowExecutor toFlowExecutor() const override;

    void fromExecutor(const ConfigFlowExecutor &executor) const override;

    void drawObject() override;

public:
    QSharedPointer<NodeRecoveryStateData> recoveryStateData;
};
