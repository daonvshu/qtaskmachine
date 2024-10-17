#include "nodestategroup.h"

NodeStateGroup::NodeStateGroup(const QSharedPointer<NodeStateGroupData> &data)
    : GraphicNode(data)
    , groupData(data)
{
    data->propData.nodeName = "状态组";
}
