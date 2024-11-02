#include "commonnodelayer.h"

#include "../objects/graphicnode.h"

#include <qdebug.h>

void CommonNodeLayer::reCacheNodeObject(const QSharedPointer<GraphicObject>& object) {
    QTransform renderTransform;
    renderTransform.scale(graphicTransform.getTransform().m11(), graphicTransform.getTransform().m22());
    object->graphicTransform = renderTransform;
    object->drawObject();
    object->data->isChanged = false;
}

void CommonNodeLayer::drawCache(const QSharedPointer<GraphicObject> &object, QPainter *painter, bool drawSelectedBox) {
    auto cacheImage = object->data->objectRenderCache;
    auto topLeft = qSharedPointerCast<GraphicNodeData>(object->data)->boundingRect.topLeft();
    topLeft = graphicTransform.toGuiPoint(topLeft);
    painter->drawPixmap(topLeft, cacheImage);

    //绘制选中框
    if (drawSelectedBox) {
        auto radius = graphicTransform.toGuiDx(9);
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::white);
        auto offset = graphicTransform.toGuiDx(8);
        painter->drawRoundedRect(QRectF(topLeft, cacheImage.size()).adjusted(-offset, -offset, offset, offset), radius, radius);
        painter->restore();
    }
}

void CommonNodeLayer::drawActiveLinkPoint(const QSharedPointer<GraphicObject> &object, QPainter *painter) {

    auto nodeObject = qSharedPointerCast<GraphicNode>(object);
    auto offset = nodeObject->nodeData->boundingRect.topLeft();

    auto drawActiveCircle = [&] (const QColor& color, const QRectF& connectPointRect) {
        painter->save();
        auto pen = painter->pen();
        pen.setWidthF(graphicTransform.toGuiDx(1.5));
        pen.setColor(color);
        painter->setPen(pen);
        auto target = connectPointRect.translated(offset);
        target = graphicTransform.toGuiPoint(target);
        const qreal shadowRadius = 6;
        target.translate(shadowRadius, shadowRadius);
        auto expandR = graphicTransform.toGuiDx(2);
        painter->drawEllipse(target.adjusted(-expandR, -expandR, expandR, expandR));
        painter->restore();
    };

    int inputLinkedIndex = nodeObject->nodeData->activeInputLinkPointIndex;
    if (inputLinkedIndex != -1) {
        auto color = nodeObject->getLinkPointColor(inputLinkedIndex, true);
        drawActiveCircle(color, nodeObject->nodeData->inputLinkPoints.at(inputLinkedIndex));
    }

    int outputLinkedIndex = nodeObject->nodeData->activeOutputLinkPointIndex;
    if (outputLinkedIndex != -1) {
        auto color = nodeObject->getLinkPointColor(outputLinkedIndex, false);
        drawActiveCircle(color, nodeObject->nodeData->outputLinkPoints.at(outputLinkedIndex));
    }
}
