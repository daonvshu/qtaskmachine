#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "nodestategroup.d.h"

class NodeStateGroup : public GraphicNode {
public:
    explicit NodeStateGroup(const QSharedPointer<NodeStateGroupData>& data);

    static NodeStateGroup* create() {
        return objectCreate<NodeStateGroup, NodeStateGroupData>();
    }

    GraphicObject* clone() const override {
        return objectCreate<NodeStateGroup, NodeStateGroupData>(*groupData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_State_Group;
    }

    void drawObject() override;

public:
    QSharedPointer<NodeStateGroupData> groupData;
};
