#include "nodeeventstate.h"

NodeEventState::NodeEventState(const QSharedPointer<NodeEventStateData> &data)
    : GraphicNode(data)
    , eventStateData(data)
{
    data->propData.nodeName = "事件触发状态";
    data->inputLinkPointColors << 0x77E000;
    data->outputLinkPointColors << 0x00E0E0 << 0xD81E06;
}

ConfigFlowExecutor NodeEventState::toFlowExecutor() const {
    auto executor = GraphicNode::toFlowExecutor();
    executor.timeout = eventStateData->eventPropData.timeout();
    executor.retry = eventStateData->eventPropData.timeoutRetryCount();
    executor.funcRetry = eventStateData->eventPropData.timeoutRetryCallback();

    return executor;
}

void NodeEventState::fromExecutor(const ConfigFlowExecutor &executor) {
    GraphicNode::fromExecutor(executor);
    eventStateData->eventPropData.timeout = executor.timeout();
    eventStateData->eventPropData.timeoutRetryCount = executor.retry();
    eventStateData->eventPropData.timeoutRetryCallback = executor.funcRetry();
}
