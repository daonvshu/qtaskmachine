#include "staticnodelayer.h"

#include "../objects/graphicnode.h"

#include <qelapsedtimer.h>
#include <qdebug.h>

StaticNodeLayer::StaticNodeLayer(QObject *parent)
    : CommonNodeLayer(parent)
{
}

void StaticNodeLayer::reload(QPainter *painter) {
    if (graphicEntries == nullptr) {
        return;
    }
    auto nodes = GraphicObject::getVisibleObjects<GraphicNode>(graphicEntries);
    for (const auto& node : nodes) {
        if (node->data->selected) { // 不绘制当前选中的节点，交给activeLayer绘制
            continue;
        }
        drawCache(node, painter);
        drawActiveLinkPoint(node, painter);
    }
}

void StaticNodeLayer::reCache() {
    if (graphicEntries == nullptr) {
        return;
    }
    bool scaleChanged = checkAndUpdateScale();
    auto nodes = GraphicObject::getVisibleObjects<GraphicNode>(graphicEntries);
    for (const auto& node : nodes) {
        if (node->data->selected) {
            continue;
        }
        if (node->data->isChanged || scaleChanged) {
            reCacheNodeObject(const_cast<GraphicNode*>(node));
        }
    }
}
