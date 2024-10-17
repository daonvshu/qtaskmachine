#pragma once

#include <qobject.h>

#include "../graphicnode.h"
#include "../../render/nodes/nodegroupstaterender.h"
#include "nodestategroup.d.h"

class NodeStateGroup : public GraphicNode {
public:
    explicit NodeStateGroup(const QSharedPointer<NodeStateGroupData>& data);

    static QSharedPointer<NodeStateGroup> create() {
        return QSharedPointer<NodeStateGroup>::create(QSharedPointer<NodeStateGroupData>::create());
    }

    GraphicRenderInterface * getRender() override {
        return new NodeGroupStateRender(groupData);
    }

    GraphicObjectType objectType() override {
        return GraphicObjectType::Node_State_Group;
    }

public:
    QSharedPointer<NodeStateGroupData> groupData;
};
