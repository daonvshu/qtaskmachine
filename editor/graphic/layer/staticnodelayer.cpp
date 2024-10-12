#include "staticnodelayer.h"

#include <qelapsedtimer.h>
#include <qdebug.h>

StaticNodeLayer::StaticNodeLayer(QObject *parent)
    : GraphicLayer(parent)
{
}

void StaticNodeLayer::reCache() {
    layerCache.fill(Qt::transparent);

    QPainter painter(&layerCache);
    painter.setRenderHint(QPainter::Antialiasing);

    //QElapsedTimer elapsedTimer;
    //elapsedTimer.start();
    for (const auto& node : staticNodeList) {
        if (node->data->selected) { // 不绘制当前选中的节点，交给activeLayer绘制
            continue;
        }
        node->graphicTransform = graphicTransform;
        node->drawGraphicObject(&painter, true);
    }
    //qDebug() << "static node layer reload used time:" << elapsedTimer.elapsed();
}
