#pragma once

#include "graphicnode.h"

#include <qsharedpointer.h>

struct GraphicLinkLineData : GraphicObjectData {
    const GraphicNode* linkFromNode = nullptr;  //链接的源节点
    const GraphicNode* linkToNode = nullptr;    //链接的目标节点

    int linkFromPointIndex = -1; //链接的源节点输出连接点index
    int linkToPointIndex = -1;   //链接的目标节点输入连接点index

    QPointF tempLinkToPoint; //临时链接目标点（鼠标点）

    bool isEditing = true; //是否正在编辑中
};