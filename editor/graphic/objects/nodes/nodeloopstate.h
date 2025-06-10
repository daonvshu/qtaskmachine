#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "nodeloopstate.d.h"

class NodeLoopState : public GraphicNode {
public:
    explicit NodeLoopState(const QSharedPointer<NodeLoopStateData>& data, bool initialData = true);

    static NodeLoopState* create() {
        return objectCreate<NodeLoopState, NodeLoopStateData>();
    }

    GraphicObject* clone() const override {
        return objectClone<NodeLoopState, NodeLoopStateData>(loopStateData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_Loop_State;
    }

    bool testLinkLineIndexValid(int linkIndex, bool isInputPoint) const override;

    ConfigFlowExecutor toFlowExecutor() const override;

    void fromExecutor(const ConfigFlowExecutor &executor) const override;

    void drawObject() override;

    bool containKeywords(const QString& keywords) const override;

public:
    QSharedPointer<NodeLoopStateData> loopStateData;
};
