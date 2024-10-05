#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "../../render/nodes/nodenormalstaterender.h"

class NodeNormalState : public GraphicNode {
public:
    explicit NodeNormalState(const QSharedPointer<GraphicNodeData>& data);

    static QSharedPointer<NodeNormalState> create() {
        return QSharedPointer<NodeNormalState>::create(QSharedPointer<GraphicNodeData>::create());
    }

    GraphicRenderInterface * getRender() override {
        return new NodeNormalStateRender(data);
    }
};
