#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "../../render/nodes/noderecoverystaterender.h"
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

    GraphicRenderInterface * getRender() override {
        return new NodeRecoveryStateRender(recoveryStateData);
    }

    GraphicObjectType objectType() override {
        return GraphicObjectType::Node_Recovery_State;
    }

public:
    QSharedPointer<NodeRecoveryStateData> recoveryStateData;
};
