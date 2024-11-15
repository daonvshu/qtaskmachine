#include "activenodelayer.h"

#include "../objects/graphicnode.d.h"

ActiveNodeLayer::ActiveNodeLayer(QObject *parent)
    : CommonNodeLayer(parent)
{
}

void ActiveNodeLayer::reload(QPainter *painter) {
    if (activeNode == nullptr) {
        return;
    }

    //更新位置
    auto& boundingRect = qSharedPointerCast<GraphicNodeData>(activeNode->data)->boundingRect;
    boundingRect.moveTop(activeNode->data->renderPosition.y());
    boundingRect.moveLeft(activeNode->data->renderPosition.x() - boundingRect.width() / 2);

    //绘制缓存图片
    drawCache(activeNode, painter, true);

    //绘制选中链接点
    drawActiveLinkPoint(activeNode, painter);
}

void ActiveNodeLayer::reCache() {
    if (activeNode == nullptr) {
        return;
    }

    bool scaleChanged = checkAndUpdateScale();
    if (activeNode->data->isChanged || scaleChanged) {
        reCacheNodeObject(activeNode);
    }
}

