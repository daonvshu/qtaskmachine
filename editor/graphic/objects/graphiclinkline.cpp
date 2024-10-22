#include "graphiclinkline.h"

#include "../utils/bezierdistancecalculator.h"

#include <qdebug.h>

GraphicLinkLine::GraphicLinkLine(const QSharedPointer<GraphicLinkLineData> &data)
    : GraphicObject(data)
    , linkData(data)
{
}

bool GraphicLinkLine::selectTest(const QPointF &point) {
    auto linkFromPoint = linkData->linkFromNode->nodeData->outputLinkPoints[linkData->linkFromPointIndex].center();
    QPointF linkToPoint;
    if (linkData->isEditing) {
        linkToPoint = linkData->tempLinkToPoint;
    } else {
        linkToPoint = linkData->linkToNode->nodeData->inputLinkPoints[linkData->linkToPointIndex].center();
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
