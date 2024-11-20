#pragma once

#include <qobject.h>

#include "../graphicnode.h"

class NodeBeginState : public GraphicNode {
public:
    explicit NodeBeginState(const QSharedPointer<GraphicNodeData>& data, bool initialData = true);

    static NodeBeginState* create() {
        return objectCreate<NodeBeginState, GraphicNodeData>();
    }

    GraphicObject* clone() const override {
        return objectClone<NodeBeginState, GraphicNodeData>(nodeData, false);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Begin_State;
    }

    void drawObject() override;
};
