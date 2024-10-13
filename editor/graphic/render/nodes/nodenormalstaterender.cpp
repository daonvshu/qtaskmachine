#include "nodenormalstaterender.h"

NodeNormalStateRender::NodeNormalStateRender(const QSharedPointer<GraphicObjectData> &data)
    : d(qSharedPointerCast<GraphicNodeData>(data))
{
}

void NodeNormalStateRender::drawObject(bool isActiveState) {
    // padding
    const int titlePadding = 12;
    const int itemPadding = 12;
    const int propertyTitlePadding = 4;
    // font size
    const int titleFontSize = 14;
    const int itemFontSize = 12;
    const int propertyTitleFontSize = 12;
    const int propertyItemFontSize = 12;
    // row height
    const int titleHeight = 40;
    const int itemHeight = 27;
    const int propertyTitleHeight = 25;
    const int propertyItemHeight = 25;
    const int splitHeight1 = 2;

    const int itemRowSize = 2;

    // calc min width
    int minTitleWidth = getTextWidthByFont(d->propData.nodeName(), titleFontSize) + titlePadding * 2;
    int minSubItemWidth = getTextWidthByFont(d->propData.funcEnter(), itemFontSize);
    minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(d->propData.funcExit(), itemFontSize));
    auto bindStrings = d->propData.propertyBindDisplayStrings();
    for (const auto& str : bindStrings) {
        minSubItemWidth = qMax(minSubItemWidth, getTextWidthByFont(str, propertyItemFontSize));
    }
    minSubItemWidth += itemPadding * 2;

    // draw node body
    int minItemHeight = itemHeight * itemRowSize;
    if (!bindStrings.isEmpty()) {
        minItemHeight += propertyTitleHeight + propertyItemHeight * bindStrings.size();
    }
    auto bodyRect = getNodeBodyRectFromTopCenter(d->renderPosition, qMax(minTitleWidth, minSubItemWidth), minItemHeight, splitHeight1);
    if (isActiveState) {
        d->boundingRect = bodyRect;
    }
    drawNodeBody(bodyRect, d);

    // draw split line
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Normal_State);

    // draw title
    auto titleDrawRect = QRectF(bodyRect.left(), bodyRect.top(), bodyRect.width(), titleHeight);
    drawNodeTitle(titleDrawRect, d->propData.nodeName(), titleFontSize, titlePadding);

    // draw enter row
    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + titleHeight + splitHeight1, bodyRect.width(), itemHeight);
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
    if (isActiveState) {
        d->outputLinkPoints.clear();
        d->outputLinkPoints << getConnectPointRect(itemExitRow, false);
    }
    drawConnectableItem(itemExitRow,
                        d->propData.funcExit().isEmpty() ? "(onExit)" : d->propData.funcExit(),
                        itemFontSize, 0x00E0E0, false,
                        d->activeOutputLinkPointIndex != -1);

    if (!bindStrings.isEmpty()) {
        QRectF propertyTitleRect(bodyRect.left(), itemExitRow.bottom(), bodyRect.width(), propertyTitleHeight);
        drawPropertyTitle(propertyTitleRect, "property set:", propertyTitleFontSize, propertyTitlePadding);
        QRectF propertyItemRect(bodyRect.left(), propertyTitleRect.bottom(), bodyRect.width(), propertyItemHeight);
        for (int i = 0; i < bindStrings.size(); ++i) {
            bool enterSetProp = d->propData.properties()[i].callOnEntered();
            QColor indicatorColor = enterSetProp ? 0x77E000 : 0x00E0E0;
            drawPropertyRow(propertyItemRect.translated(0, i * propertyItemHeight), bindStrings[i], propertyItemFontSize, indicatorColor);
        }
    }
}
