#include "nodenormalstaterender.h"

NodeNormalStateRender::NodeNormalStateRender(const QSharedPointer<GraphicObjectData> &data)
    : d(qSharedPointerCast<GraphicNodeData>(data))
{
}

void NodeNormalStateRender::drawObject(bool isActiveState) {
    int minTitleWidth = getTextWidthByFont(d->nodeName, 14) + 12 * 2;
    int minSubItemWidth = 0;
    minSubItemWidth = qMax(getTextWidthByFont(d->funcEnter, 12), getTextWidthByFont(d->funcExit, 12));
    minSubItemWidth += 12 * 2;

    auto bodyRect = getNodeBodyRectFromTopCenter(d->renderPosition, qMax(minTitleWidth, minSubItemWidth), 33 * 2);
    drawNodeBody(bodyRect, d->selected);
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Normal_State);

    // draw title
    auto titleDrawRect = QRectF(bodyRect.left(), bodyRect.top(), bodyRect.width(), 40);
    drawNodeTitle(titleDrawRect, d->nodeName, 14);

    QRectF itemEnterRow(bodyRect.left(), bodyRect.top() + 40 + 2, bodyRect.width(), 33);
    drawConnectableItem(itemEnterRow, d->funcEnter.isEmpty() ? "(onEnter)" : d->funcEnter, 12, Qt::green, true);

    QRectF itemExitRow = itemEnterRow.translated(0, 33);
    drawConnectableItem(itemExitRow, d->funcExit.isEmpty() ? "(onExit)" : d->funcExit, 12, Qt::green, false);
}
