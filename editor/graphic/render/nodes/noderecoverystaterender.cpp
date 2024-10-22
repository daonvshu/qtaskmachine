#include "noderecoverystaterender.h"

NodeRecoveryStateRender::NodeRecoveryStateRender(const QSharedPointer<NodeRecoveryStateData> &data)
    : CommonNodeStateRender(data)
    , d(data)
{
}

void NodeRecoveryStateRender::drawObject(bool isActiveState) {

    // calc min item width
    int minSubItemWidth = functionEnterWidth();
    QStringList bindStrings;
    minSubItemWidth = qMax(minSubItemWidth, minPropertyWidth(bindStrings));
    minSubItemWidth += itemPadding * 2;

    // calc min item height
    int minItemHeight = itemHeight + propertyItemHeight;
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }

    // draw node body
    auto bodyRect = renderNodeBody(minSubItemWidth, minItemHeight, isActiveState);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Recovery_State);

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

    // draw delay data row
    QRectF itemDataRow(bodyRect.left(), itemEnterRow.bottom(), bodyRect.width(), propertyItemHeight);
    drawPropertyRow(itemDataRow, d->recoveryPropData.nested() ? "嵌套" : "非嵌套", itemFontSize, d->inputLinkPointColors.first(), true);

    // draw property rows
    renderPropertyItems(bodyRect, itemDataRow.bottom(), bindStrings);
}
