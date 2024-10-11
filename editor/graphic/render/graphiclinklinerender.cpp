#include "graphiclinklinerender.h"

#include "../objects/graphicnode.d.h"

#include <qpainterpath.h>
#include <qdebug.h>

GraphicLinkLineRender::GraphicLinkLineRender(const QSharedPointer<GraphicLinkLineData> &data)
    : d(data)
{
}

void GraphicLinkLineRender::drawObject(bool isActiveState) {
    auto linkFromPoint = qSharedPointerCast<GraphicNodeData>(d->linkFromNodeData)->outputLinkPoints[d->linkFromPointIndex].center();
    QPointF linkToPoint;
    if (d->isEditing) {
        linkToPoint = d->tempLinkToPoint;
    } else {
        linkToPoint = qSharedPointerCast<GraphicNodeData>(d->linkToNodeData)->inputLinkPoints[d->linkToPointIndex].center();
    }

    linkFromPoint = graphicTransform.toGuiPoint(linkFromPoint);
    linkToPoint = graphicTransform.toGuiPoint(linkToPoint);

    auto length = QLineF(linkFromPoint, linkToPoint).length();
    auto controlPoint1 = linkFromPoint;
    controlPoint1 += QPointF(length * 0.25, 0);
    auto controlPoint2 = linkToPoint;
    controlPoint2 -= QPointF(length * 0.25, 0);

    QPainterPath path;
    path.moveTo(linkFromPoint);
    path.cubicTo(controlPoint1, controlPoint2, linkToPoint);

    auto pen = renderPainter->pen();
    pen.setWidthF(graphicTransform.toGuiDx(2));
    pen.setColor(Qt::white);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    renderPainter->save();
    renderPainter->setPen(pen);
    renderPainter->drawPath(path);
    renderPainter->restore();
}
