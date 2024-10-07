#include "nodenormalstaterender.h"

NodeNormalStateRender::NodeNormalStateRender(const QSharedPointer<GraphicObjectData> &data)
    : d(qSharedPointerCast<GraphicNodeData>(data))
{
}

void NodeNormalStateRender::drawObject(bool isActiveState) {
    int minTitleWidth = getTextWidthByFont(d->propData.nodeName(), 14) + 12 * 2;
    int minSubItemWidth = 0;
    minSubItemWidth = qMax(getTextWidthByFont(d->propData.funcEnter(), 12), getTextWidthByFont(d->propData.funcExit(), 12));
    minSubItemWidth += 12 * 2;

    auto bodyRect = getNodeBodyRectFromTopCenter(d->renderPosition, qMax(minTitleWidth, minSubItemWidth), 33 * 2);
    d->boundingRect = bodyRect;
    drawNodeBody(bodyRect, d);

    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Normal_State);

    // draw title
    auto titleDrawRect = QRectF(bodyRect.left(), bodyRect.top(), bodyRect.width(), 40);
    drawNodeTitle(titleDrawRect, d->propData.nodeName(), 14);

    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + 40 + 2, bodyRect.width(), 33);
    drawConnectableItem(itemEnterRow, d->propData.funcEnter().isEmpty() ? "(onEnter)" : d->propData.funcEnter(), 12, 0x77E000, true);

    QRectF itemExitRow = itemEnterRow.translated(0, 33);
    drawConnectableItem(itemExitRow, d->propData.funcExit().isEmpty() ? "(onExit)" : d->propData.funcExit(), 12, 0x00E0E0, false);
}
