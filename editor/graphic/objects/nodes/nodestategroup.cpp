#include "nodestategroup.h"

NodeStateGroup::NodeStateGroup(const QSharedPointer<NodeStateGroupData> &data)
    : GraphicNode(data)
    , groupData(data)
{
    data->propData.nodeName = "状态组";
    data->inputLinkPointColors << 0x77E000;
    data->outputLinkPointColors << 0xD81E06 << 0xD165A2 << 0x00E0E0;
}
