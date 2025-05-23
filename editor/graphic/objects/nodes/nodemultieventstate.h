#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "nodemultieventstate.d.h"

class NodeMultiEventState : public GraphicNode {
public:
    explicit NodeMultiEventState(const QSharedPointer<NodeMultiEventStateData>& data, bool initialData = true);

    static NodeMultiEventState* create() {
        return objectCreate<NodeMultiEventState, NodeMultiEventStateData>();
    }

    GraphicObject* clone() const override {
        return objectClone<NodeMultiEventState, NodeMultiEventStateData>(eventStateData, false);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_MultiEvent_State;
    }

    bool testLinkLineIndexValid(int linkIndex, bool isInputPoint) const override;

    void fromExecutor(const ConfigFlowExecutor &executor) const override;

    void drawObject() override;

    bool containKeywords(const QString& keywords) const override;

public:
    QSharedPointer<NodeMultiEventStateData> eventStateData;
};
