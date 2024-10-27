#include "nodedelaystate.h"

NodeDelayState::NodeDelayState(const QSharedPointer<NodeDelayStateData> &data)
    : GraphicNode(data)
    , delayStateData(data)
{
    data->propData.nodeName = "延时状态";
    data->inputLinkPointColors << 0x77E000;
    data->outputLinkPointColors << 0x00E0E0;
}

ConfigFlowExecutor NodeDelayState::toFlowExecutor() const {
    auto executor = GraphicNode::toFlowExecutor();
    executor.delay = delayStateData->delayPropData.delayMs();

    return executor;
}

void NodeDelayState::fromExecutor(const ConfigFlowExecutor &executor) {
    GraphicNode::fromExecutor(executor);
    delayStateData->delayPropData.delayMs = executor.delay();
}
