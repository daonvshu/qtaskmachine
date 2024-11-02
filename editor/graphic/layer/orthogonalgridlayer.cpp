#include "orthogonalgridlayer.h"

#include <qpainter.h>
#include <qdebug.h>
#include <qmath.h>

OrthogonalGridLayer::OrthogonalGridLayer(QObject *parent)
    : GraphicLayer(parent)
    , gridEnabled(true)
{
}

void OrthogonalGridLayer::reload(QPainter *painter) {
    if (boxCache.isNull()) {
        return;
    }
    auto center = graphicTransform.toGuiPoint(QPointF(0, 0));
    //移动到0的左边
    while (center.x() > 0) {
        center.setX(center.x() - boxCache.width());
    }
    while (center.x() + boxCache.width() < 0) {
        center.setX(center.x() + boxCache.width());
    }
    //移动到0的上边
    while (center.y() > 0) {
        center.setY(center.y() - boxCache.height());
    }
    while (center.y() + boxCache.height() < 0) {
        center.setY(center.y() + boxCache.height());
    }
    //绘制所有box
    for (int row = qRound(center.y()); row <= viewSize.height(); row += boxCache.height()) {
        for (int col = qRound(center.x()); col <= viewSize.width(); col += boxCache.width()) {
            painter->drawPixmap(col, row, boxCache);
        }
    }
}

void OrthogonalGridLayer::reCache() {

    if (!gridEnabled) {
        return;
    }

    bool scaleChanged = checkAndUpdateScale();
    if (!scaleChanged) {
        return;
    }

    const qreal minimumGridWidth = 1.0e-8;
    const qreal minGridSpacing = 10;

    gridWidth = minimumGridWidth;
    while (graphicTransform.toGuiDx(gridWidth) < minGridSpacing) {
        gridWidth *= 10;
    }

    gridWidth = graphicTransform.toGuiDx(gridWidth);
    qreal metaGridWith = gridWidth * 5;

    boxCache = QPixmap(qCeil(metaGridWith), qCeil(metaGridWith));
    boxCache.fill(Qt::transparent);
    QPainter painter(&boxCache);

    drawGridLine(&painter, gridWidth, gridColor());
    drawGridLine(&painter, metaGridWith, metaGridColor());
}

void OrthogonalGridLayer::drawGridLine(QPainter *painter, qreal width, const QColor& color) {
    painter->setPen(color);
    for (qreal r = 0; r <= boxCache.height(); r += width) {
        QLineF verticalLine(0, r, boxCache.width(), r);
        painter->drawLine(verticalLine);
    }
    for (qreal c = 0; c <= boxCache.width(); c += width) {
        QLineF horizontalLine(c, 0, c, boxCache.height());
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
