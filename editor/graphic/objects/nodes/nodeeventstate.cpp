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

void NodeEventState::fromExecutor(const ConfigFlowExecutor &executor) const {
    GraphicNode::fromExecutor(executor);
    eventStateData->eventPropData.timeout = executor.timeout();
    eventStateData->eventPropData.timeoutRetryCount = executor.retry();
    eventStateData->eventPropData.timeoutRetryCallback = executor.funcRetry();
}

void NodeEventState::drawObject() {

    // calc min item width
    int minSubItemWidth = qMax(functionEnterWidth(), functionExitWidth());
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(eventStateData->eventPropData.timeoutRetryCallback(), itemFontSize));

    bool timeoutEnabled = eventStateData->eventPropData.timeout() != 0;
    bool timeoutRetryEnabled = eventStateData->eventPropData.timeoutRetryCount() != 0;
    bool timeoutCallbackEnabled = !eventStateData->eventPropData.timeoutRetryCallback().isEmpty();
    //...
    QString timeoutData = QString("超时时间：%1 ms").arg(eventStateData->eventPropData.timeout());
    if (timeoutEnabled) {
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(timeoutData, itemFontSize));
    }
    QString retryCountData = QString("重试次数：%1").arg(eventStateData->eventPropData.timeoutRetryCount());
    if (timeoutRetryEnabled) {
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(retryCountData, itemFontSize));
    }
    QString retryCallbackData = QString("重试回调：%1").arg(eventStateData->eventPropData.timeoutRetryCallback());
    if (timeoutCallbackEnabled) {
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(retryCallbackData, itemFontSize));
    }
    //...
    EventTriggerFunction* eventRows[2] = { &eventStateData->eventPropData.normalEvent(),
                                           &eventStateData->eventPropData.errorEvent() };
    for (auto & eventRow : eventRows) {
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(eventRow->triggerFunc(), itemFontSize));
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(eventRow->checkFunc(), itemFontSize - 3));
    }
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight * (2 + 2);
    if (timeoutEnabled) {
        minItemHeight += propertyItemHeight;
    }
    if (timeoutRetryEnabled) {
        minItemHeight += propertyItemHeight;
    }
    if (timeoutCallbackEnabled) {
        minItemHeight += propertyItemHeight;
    }
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Event_State);

    // draw title
    renderNodeTitle(bodyRect);

    // draw enter row
    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight, bodyRect.width(), itemHeight);
    eventStateData->inputLinkPoints.clear();
    eventStateData->inputLinkPoints << getConnectPointRect(itemEnterRow, true);
    drawConnectableItem(itemEnterRow,
                        eventStateData->propData.funcEnter().isEmpty() ? "(onEnter)" : eventStateData->propData.funcEnter(),
                        itemFontSize, eventStateData->inputLinkPointColors.first(), true);

    // draw exit row
    QRectF itemExitRow = itemEnterRow.translated(0, itemHeight);
    auto outputColor = eventStateData->outputLinkPointColors.first();
    drawPropertyRow(itemExitRow,
                    eventStateData->propData.funcExit().isEmpty() ? "(onExit)" : eventStateData->propData.funcExit(),
                    itemFontSize, outputColor, false);

    // draw normal trigger event row
    eventStateData->outputLinkPoints.clear();
    for (int i = 0; i < 2; ++i) {
        QRectF itemEventRow = itemExitRow.translated(0, itemHeight * (i + 1));
        eventStateData->outputLinkPoints << getConnectPointRect(itemEventRow, false);
        drawDoubleRowConnectableItem(itemEventRow,
                                     eventRows[i]->triggerFunc().isEmpty() ? (i == 0 ? "(Normal)" : "(Error)") : eventRows[i]->triggerFunc(),
                                     eventRows[i]->checkFunc(),
                                     itemFontSize, eventStateData->outputLinkPointColors.at(i), false);
    }

    QRectF nextRowRect = itemExitRow.translated(0, itemHeight * 3 - propertyItemHeight);
    nextRowRect.setHeight(propertyItemHeight);
    // draw extra data
    if (timeoutEnabled) {
        QRectF timeoutDataRow = nextRowRect.translated(0, propertyItemHeight);
        nextRowRect = timeoutDataRow;
        drawPropertyRow(timeoutDataRow, timeoutData, itemFontSize, outputColor, true);
    }

    if (timeoutRetryEnabled) {
        QRectF retryCountDataRow = nextRowRect.translated(0, propertyItemHeight);
        nextRowRect = retryCountDataRow;
        drawPropertyRow(retryCountDataRow, retryCountData, itemFontSize, outputColor, true);
    }

    if (timeoutCallbackEnabled) {
        QRectF retryCallbackRow = nextRowRect.translated(0, propertyItemHeight);
        nextRowRect = retryCallbackRow;
        drawPropertyRow(retryCallbackRow, retryCallbackData, itemFontSize, outputColor, true);
    }

    // draw property rows
    renderPropertyItems(bodyRect, nextRowRect.bottom(), bindStrings);

    GraphicRenderInterface::drawObject();
}
