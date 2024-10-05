#include "orthogonalgridlayer.h"

#include <qpainter.h>
#include <qdebug.h>
#include <qmath.h>

OrthogonalGridLayer::OrthogonalGridLayer(QObject *parent)
    : GraphicLayer(parent)
    , gridEnabled(true)
{
}

void OrthogonalGridLayer::reCache() {
    //TODO: cache block grid
    layerCache.fill(Qt::transparent);
    QPainter painter(&layerCache);

    if (!gridEnabled) {
        return;
    }

    const qreal minimumGridWidth = 1.0e-8;
    const qreal minGridSpacing = 10;

    gridWidth = minimumGridWidth;
    while (graphicTransform.toGuiDx(gridWidth) < minGridSpacing) {
        gridWidth *= 10;
    }

    //qDebug() << "gui spacing:" << toGuiDx(gridWidth);
    qreal metaGridWith = gridWidth * 5;

    drawGridLine(&painter, gridWidth, gridColor());
    drawGridLine(&painter, metaGridWith, metaGridColor());
}

void OrthogonalGridLayer::drawGridLine(QPainter *painter, qreal gridWidth, const QColor& color) {
    auto viewRect = graphicTransform.toRealPoint(layerCache.rect());
    viewRect.setLeft(qFloor(viewRect.left() / gridWidth) * gridWidth);
    viewRect.setRight(qCeil(viewRect.right() / gridWidth) * gridWidth);
    viewRect.setTop(qFloor(viewRect.top() / gridWidth) * gridWidth);
    viewRect.setBottom(qCeil(viewRect.bottom() / gridWidth) * gridWidth);

    int numberX = qRound(viewRect.width() / gridWidth) + 1;
    int numberY = qRound(viewRect.height() / gridWidth) + 1;
    int number = numberX * numberY;

    const int maxGridPoints = 1000000;
    if (number <= 0 || number > maxGridPoints) {
        return;
    }

    //qDebug() << "view rect:" << viewRect;
    painter->setPen(color);
    for (qreal r = viewRect.top(); r <= viewRect.bottom(); r += gridWidth) {
        QLineF verticalLine(viewRect.left(), r, viewRect.right(), r);
        verticalLine = graphicTransform.toGuiPoint(verticalLine);
        painter->drawLine(verticalLine);
    }
    for (qreal c = viewRect.left(); c <= viewRect.right(); c += gridWidth) {
        QLineF horizontalLine(c, viewRect.top(), c, viewRect.bottom());
        horizontalLine = graphicTransform.toGuiPoint(horizontalLine);
        painter->drawLine(horizontalLine);
    }
}

QColor OrthogonalGridLayer::gridColor() const {
    return 0x353A3F;
}

QColor OrthogonalGridLayer::metaGridColor() const {
    return 0x3A3F44;
}

void OrthogonalGridLayer::snapTest(QPointF& point) const {
    point.setX(qRound(point.x() / gridWidth) * gridWidth);
    point.setY(qRound(point.y() / gridWidth) * gridWidth);
}
