#pragma once

#include <qobject.h>

#include "graphicnode.d.h"

#include "../graphicobject.h"

class GraphicNode : public GraphicObject {
public:
    explicit GraphicNode(const QSharedPointer<GraphicNodeData>& data);

    bool selectTest(const QPointF& point) override;

protected:
    QSharedPointer<GraphicNodeData> nodeData;
};
