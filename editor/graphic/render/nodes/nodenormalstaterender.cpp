#include "nodenormalstaterender.h"

NodeNormalStateRender::NodeNormalStateRender(const QSharedPointer<GraphicObjectData> &data)
    : d(qSharedPointerCast<GraphicNodeData>(data))
{
}

void NodeNormalStateRender::drawObject(bool isActiveState) {
    auto titleFont = renderPainter->font();
    titleFont.setPixelSize(14);
    auto titleFontMetrics = QFontMetrics(titleFont);
    int minTitleWidth = titleFontMetrics.horizontalAdvance(d->nodeName) + 8 * 2;

    auto subItemFont = renderPainter->font();
    subItemFont.setPixelSize(12);
    auto subItemFontMetrics = QFontMetrics(subItemFont);
    int minSubItemWidth = INT32_MAX;
    minSubItemWidth = qMin(minSubItemWidth, subItemFontMetrics.horizontalAdvance(d->funcEnter));
    minSubItemWidth = qMin(minSubItemWidth, subItemFontMetrics.horizontalAdvance(d->funcExit));
    minSubItemWidth += 8 * 2;

    auto bodyRect = drawNodeBody(d->renderPosition, qMax(minTitleWidth, minSubItemWidth), 32 * 2, d->selected);
    drawNodeSplitLine(bodyRect, GraphicObjectType::Node_Normal_State);
}
