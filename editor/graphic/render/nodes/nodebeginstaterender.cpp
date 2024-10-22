#include "nodebeginstaterender.h"

NodeBeginStateRender::NodeBeginStateRender(const QSharedPointer<GraphicObjectData> &data)
    : CommonNodeStateRender(data)
    , d(qSharedPointerCast<GraphicNodeData>(data))
{
}

void NodeBeginStateRender::drawObject(bool isActiveState) {

    // calc min item width
    int minSubItemWidth = functionExitWidth();
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
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Begin_State);

    // draw title
    renderNodeTitle(bodyRect);

    // draw enter row
    QRectF itemExitRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight, bodyRect.width(), itemHeight);
    if (isActiveState) {
        d->outputLinkPoints.clear();
        d->outputLinkPoints << getConnectPointRect(itemExitRow, false);
    }
    drawConnectableItem(itemExitRow,
                        d->propData.funcExit().isEmpty() ? "(onExit)" : d->propData.funcExit(),
                        itemFontSize, d->outputLinkPointColors.first(), false,
                        d->activeOutputLinkPointIndex != -1);

    // draw property rows
    renderPropertyItems(bodyRect, itemExitRow.bottom(), bindStrings);
}
