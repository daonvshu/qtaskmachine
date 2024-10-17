#include "nodemultieventstaterender.h"

NodeMultiEventStateRender::NodeMultiEventStateRender(const QSharedPointer<NodeMultiEventStateData> &data)
    : CommonNodeStateRender(data)
    , d(data)
{
}

void NodeMultiEventStateRender::drawObject(bool isActiveState) {
    // calc min item width
    int minSubItemWidth = qMax(functionEnterWidth(), functionExitWidth());
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    auto eventRows = d->eventPropData.events();
    for (auto & eventRow : eventRows) {
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(eventRow.triggerFunc() + QString("[%1]").arg(eventRow.branchId()), itemFontSize));
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(eventRow.checkFunc(), itemFontSize - 3));
    }
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight * (2 + eventRows.size());
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
    for (int i = 0; i < eventRows.size(); ++i) {
        QRectF itemEventRow = itemExitRow.translated(0, itemHeight * (i + 1));
        if (isActiveState) {
            d->outputLinkPoints << getConnectPointRect(itemEventRow, false);
        }
        QString mainEventText = QString("%0[%1]").arg(eventRows[i].triggerFunc()).arg(eventRows[i].branchId());
        drawDoubleRowConnectableItem(itemEventRow,
                                     mainEventText,
                                     eventRows[i].checkFunc(),
                                     itemFontSize, 0x00E0E0, false,
                                     d->activeOutputLinkPointIndex == i);
    }

    // draw property rows
    renderPropertyItems(bodyRect, itemExitRow.bottom() + itemHeight * eventRows.size(), bindStrings);
}
