#pragma once

#include "../../graphicobject.d.h"

#include "../graphicnode.h"

struct NodeMoveActionData : GraphicObjectData {
    QList<const GraphicNode*> targets;

    QList<QPointF> renderPositions; // 绘图位置
    QList<QPointF> oldRenderPositions; // 旧的绘图位置

    explicit NodeMoveActionData(const QList<const GraphicNode*>& targets): targets(targets) {}
};