#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "noderecoverystate.d.h"

class NodeRecoveryState : public GraphicNode {
public:
    explicit NodeRecoveryState(const QSharedPointer<NodeRecoveryStateData>& data);

    static QSharedPointer<NodeRecoveryState> create() {
        return objectCreate<NodeRecoveryState, NodeRecoveryStateData>();
    }

    QSharedPointer<GraphicObject> clone() override {
        return objectCreate<NodeRecoveryState, NodeRecoveryStateData>(*recoveryStateData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Recovery_State;
    }

    ConfigFlowExecutor toFlowExecutor() const override;

    void fromExecutor(const ConfigFlowExecutor &executor) override;

    void drawObject() override;

public:
    QSharedPointer<NodeRecoveryStateData> recoveryStateData;
};
