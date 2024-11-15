#include "staticnodelayer.h"

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
    for (int i = 0; i < graphicEntries->index(); i++) {
        auto node = dynamic_cast<const GraphicObject*>(graphicEntries->command(i));
        if (!isValidObject(node)) {
            continue;
        }
        drawCache(node, painter);
        drawActiveLinkPoint(node, painter);
    }
}

void StaticNodeLayer::reCache() {
    bool scaleChanged = checkAndUpdateScale();
    for (int i = 0; i < graphicEntries->index(); i++) {
        auto node = dynamic_cast<const GraphicObject*>(graphicEntries->command(i));
        if (!isValidObject(node)) {
            continue;
        }
        if (node->data->isChanged || scaleChanged) {
            reCacheNodeObject(const_cast<GraphicObject*>(node));
        }
    }
}

bool StaticNodeLayer::isValidObject(const GraphicObject* obj) {
    if (obj == nullptr) {
        return false;
    }
    if (obj->data->assignRemoved) {
        return false;
    }
    if (obj->data->selected) { // 不绘制当前选中的节点，交给activeLayer绘制
        return false;
    }
    if (obj->objectType() == GraphicObjectType::Link_Line) {
        return false;
    }
    return true;
}
