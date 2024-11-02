#include "staticnodelayer.h"

#include <qelapsedtimer.h>
#include <qdebug.h>

StaticNodeLayer::StaticNodeLayer(QObject *parent)
    : CommonNodeLayer(parent)
{
}

void StaticNodeLayer::reload(QPainter *painter) {
    for (const auto& node : staticNodeList) {
        if (node->data->selected) { // 不绘制当前选中的节点，交给activeLayer绘制
            continue;
        }
        drawCache(node, painter);
        drawActiveLinkPoint(node, painter);
    }
}

void StaticNodeLayer::reCache() {
    bool scaleChanged = checkAndUpdateScale();
    for (const auto& node : staticNodeList) {
        if (node->data->selected) { // 不绘制当前选中的节点，交给activeLayer绘制
            continue;
        }
        if (node->data->isChanged || scaleChanged) {
            reCacheNodeObject(node);
        }
    }
}
