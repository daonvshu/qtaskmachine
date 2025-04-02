#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "nodestategroup.d.h"

class NodeStateGroup : public GraphicNode {
public:
    explicit NodeStateGroup(const QSharedPointer<NodeStateGroupData>& data, bool initialData = true);

    static NodeStateGroup* create() {
        return objectCreate<NodeStateGroup, NodeStateGroupData>();
    }

    GraphicObject* clone() const override {
        return objectClone<NodeStateGroup, NodeStateGroupData>(groupData, false);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Node_State_Group;
    }

    void drawObject() override;

    bool containKeywords(const QString& keywords) const override;

public:
    QSharedPointer<NodeStateGroupData> groupData;
};
