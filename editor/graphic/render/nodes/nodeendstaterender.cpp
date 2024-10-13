#include "nodeendstaterender.h"

NodeEndStateRender::NodeEndStateRender(const QSharedPointer<GraphicObjectData> &data)
    : CommonNodeStateRender(data)
    , d(qSharedPointerCast<GraphicNodeData>(data))
{
}

void NodeEndStateRender::drawObject(bool isActiveState) {

    // calc min item width
    int minSubItemWidth = functionEnterWidth();
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight;
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight, isActiveState);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_End_State);

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

    // draw property rows
    renderPropertyItems(bodyRect, itemEnterRow.bottom(), bindStrings);
}
