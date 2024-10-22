#include "nodenormalstaterender.h"

NodeNormalStateRender::NodeNormalStateRender(const QSharedPointer<GraphicObjectData> &data)
    : CommonNodeStateRender(data)
    , d(qSharedPointerCast<GraphicNodeData>(data))
{
}

void NodeNormalStateRender::drawObject(bool isActiveState) {

    // calc min item width
    int minSubItemWidth = qMax(functionEnterWidth(), functionExitWidth());
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight * 2;
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight, isActiveState);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Normal_State);

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
                        itemFontSize, d->inputLinkPointColors.first(), true,
                        d->activeInputLinkPointIndex != -1);

    // draw exit row
    QRectF itemExitRow = itemEnterRow.translated(0, itemHeight);
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
