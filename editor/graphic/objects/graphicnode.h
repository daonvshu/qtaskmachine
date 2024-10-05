#pragma once

#include <qobject.h>

#include "graphicnode.d.h"

#include "../graphicobject.h"

class GraphicNode : public GraphicObject {
public:
    explicit GraphicNode(const QSharedPointer<GraphicNodeData>& data);

protected:
    QSharedPointer<GraphicNodeData> nodeData;
};
