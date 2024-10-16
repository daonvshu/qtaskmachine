#include "nodeeventstaterender.h"

NodeEventStateRender::NodeEventStateRender(const QSharedPointer<NodeEventStateData> &data)
    : CommonNodeStateRender(data)
    , d(data)
{
}

void NodeEventStateRender::drawObject(bool isActiveState) {

    // calc min item width
    int minSubItemWidth = qMax(functionEnterWidth(), functionExitWidth());
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(d->eventPropData.timeoutRetryCallback(), itemFontSize));
    //...
    QString timeoutData = QString("超时时间：%1 ms").arg(d->eventPropData.timeout());
    minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(timeoutData, itemFontSize));
    QString retryCountData = QString("重试次数：%1").arg(d->eventPropData.timeoutRetryCount());
    minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(retryCountData, itemFontSize));
    QString retryCallbackData = QString("重试回调：%1").arg(d->eventPropData.timeoutRetryCallback());
    minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(retryCallbackData, itemFontSize));
    //...
    EventTriggerFunction* eventRows[2] = { &d->eventPropData.normalEvent(),
                                           &d->eventPropData.errorEvent() };
    for (auto & eventRow : eventRows) {
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(eventRow->triggerFunc(), itemFontSize));
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(eventRow->checkFunc(), itemFontSize - 3));
    }
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight * (2 + 2) + propertyItemHeight * 3;
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight, isActiveState);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Event_State);

    // draw title
    renderNodeTitle(bodyRect);

    // draw enter row
    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight, bodyRect.width(), itemHeight);
    if (isActiveState) {
        d->inputLinkPoints.clear();
        d->inputLinkPoints << getConnectPointRect(itemEnterRow, true);
    }
    drawConnectableItem(itemEnterRow,
                        d->propData.funcEnter().isEmpty() ? "(onEnter)" : d->propData.funcEnter(),
                        itemFontSize, 0x77E000, true,
                        d->activeInputLinkPointIndex != -1);

    // draw exit row
    QRectF itemExitRow = itemEnterRow.translated(0, itemHeight);
    drawPropertyRow(itemExitRow,
                    d->propData.funcExit().isEmpty() ? "(onExit)" : d->propData.funcExit(),
                    itemFontSize, 0x00E0E0, false);

    // draw normal trigger event row
    if (isActiveState) {
        d->outputLinkPoints.clear();
    }
    for (int i = 0; i < 2; ++i) {
        QRectF itemEventRow = itemExitRow.translated(0, itemHeight * (i + 1));
        if (isActiveState) {
            d->outputLinkPoints << getConnectPointRect(itemEventRow, false);
        }
        drawDoubleRowConnectableItem(itemEventRow,
                            eventRows[i]->triggerFunc().isEmpty() ? (i == 0 ? "(Normal)" : "(Error)") : eventRows[i]->triggerFunc(),
                            eventRows[i]->checkFunc(),
                            itemFontSize, i == 0 ? 0x00E0E0 : 0xD81E06, false,
                            d->activeOutputLinkPointIndex == i);
    }

    // draw extra data
    QRectF timeoutDataRow = itemExitRow.translated(0, itemHeight * 3);
    timeoutDataRow.setHeight(propertyItemHeight);
    drawPropertyRow(timeoutDataRow, timeoutData, itemFontSize, 0x00E0E0, true);

    QRectF retryCountDataRow = timeoutDataRow.translated(0, propertyItemHeight);
    drawPropertyRow(retryCountDataRow, retryCountData, itemFontSize, 0x00E0E0, true);

    QRectF retryCallbackRow = retryCountDataRow.translated(0, propertyItemHeight);
    drawPropertyRow(retryCallbackRow, retryCallbackData, itemFontSize, 0x00E0E0, true);

    // draw property rows
    renderPropertyItems(bodyRect, retryCallbackRow.bottom(), bindStrings);
}
