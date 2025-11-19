#include "graphiclinkline.h"

#include "../utils/bezierdistancecalculator.h"

#include <qpainterpath.h>
#include <qdebug.h>

GraphicLinkLine::GraphicLinkLine(const QSharedPointer<GraphicLinkLineData> &data)
    : GraphicObject(data)
    , linkData(data)
{
}

bool GraphicLinkLine::selectTest(const QPointF &point) const {
    if (linkData->linkFromPointIndex >= linkData->linkFromNode->nodeData->outputLinkPoints.size()) {
        return false;
    }
    auto linkFromPoint = linkData->linkFromNode->nodeData->outputLinkPoints[linkData->linkFromPointIndex].center();
    linkFromPoint += linkData->linkFromNode->nodeData->boundingRect.topLeft();
    QPointF linkToPoint;
    if (linkData->isEditing) {
        linkToPoint = linkData->tempLinkToPoint;
    } else {
        if (linkData->linkToPointIndex >= linkData->linkToNode->nodeData->inputLinkPoints.size()) {
            return false;
        }
        linkToPoint = linkData->linkToNode->nodeData->inputLinkPoints[linkData->linkToPointIndex].center();
        linkToPoint += linkData->linkToNode->nodeData->boundingRect.topLeft();
    }

    auto length = QLineF(linkFromPoint, linkToPoint).length();
    auto controlPoint1 = linkFromPoint;
    controlPoint1 += QPointF(length * 0.25, 0);
    auto controlPoint2 = linkToPoint;
    controlPoint2 -= QPointF(length * 0.25, 0);

    BezierCurve curve{ linkFromPoint, controlPoint1, controlPoint2, linkToPoint };
    auto distance = BezierDistanceCalculator::distanceToCurve(point, curve);
    distance = graphicTransform.toGuiDx(distance);
    //qDebug() << "link line distance:" << distance;
    return distance < 10;
}

void GraphicLinkLine::drawObject() {
    if (linkData->linkFromPointIndex >= linkData->linkFromNode->nodeData->outputLinkPoints.size()) {
        return;
    }
    auto linkFromPoint = linkData->linkFromNode->nodeData->outputLinkPoints[linkData->linkFromPointIndex].center();
    linkFromPoint += linkData->linkFromNode->nodeData->boundingRect.topLeft();
    QPointF linkToPoint;
    if (linkData->isEditing) {
        linkToPoint = linkData->tempLinkToPoint;
    } else {
        if (linkData->linkToPointIndex >= linkData->linkToNode->nodeData->inputLinkPoints.size()) {
            return;
        }
        linkToPoint = linkData->linkToNode->nodeData->inputLinkPoints[linkData->linkToPointIndex].center();
        linkToPoint += linkData->linkToNode->nodeData->boundingRect.topLeft();
    }

    const qreal shadowRadius = 6;
    linkFromPoint = graphicTransform.toGuiPoint(linkFromPoint);
    linkFromPoint += QPointF(shadowRadius, shadowRadius);
    linkToPoint = graphicTransform.toGuiPoint(linkToPoint);
    linkToPoint += QPointF(shadowRadius, shadowRadius);

    auto length = QLineF(linkFromPoint, linkToPoint).length();
    auto controlPoint1 = linkFromPoint;
    controlPoint1 += QPointF(length * 0.25, 0);
    auto controlPoint2 = linkToPoint;
    controlPoint2 -= QPointF(length * 0.25, 0);

    QPainterPath path;
    path.moveTo(linkFromPoint);
    path.cubicTo(controlPoint1, controlPoint2, linkToPoint);

    auto linkColor = dynamic_cast<const GraphicNode*>(linkData->linkFromNode)->getLinkPointColor(linkData->linkFromPointIndex, false);
    if (!linkData->selected) {
        linkColor = linkColor.darker(150);
    }
    auto pen = renderPainter->pen();
    pen.setWidthF(graphicTransform.toGuiDx(2));
    pen.setColor(linkColor);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    renderPainter->save();
    renderPainter->setPen(pen);
    renderPainter->drawPath(path);

    auto pos = path.pointAtPercent(qMin(1.0, animDrawPercent));
    animDrawPercent += 0.00833;
    if (animDrawPercent > 1.5) {
        animDrawPercent = 0;
    }
    linkColor = linkColor.lighter();
    pen.setColor(linkColor);
    pen.setWidthF(graphicTransform.toGuiDx(3));
    renderPainter->setPen(pen);
    renderPainter->drawPoint(pos);

    renderPainter->restore();
}
