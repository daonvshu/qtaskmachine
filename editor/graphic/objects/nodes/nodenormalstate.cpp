#include "nodenormalstate.h"

NodeNormalState::NodeNormalState(const QSharedPointer<GraphicNodeData> &data)
    : GraphicNode(data)
{
    data->propData.nodeName = "普通状态";
}
