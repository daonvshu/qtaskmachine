#pragma once

#include "flowchart/elements/fcbranchitem.h"
#include "flowchart/elements/fciconitem.h"
#include "flowchart/elements/fcexecutoritem.h"
#include "flowchart/elements/fcconnectline.h"

class StateConfigInterface {
public:
    virtual void setActiveItem(FlowChartExecutorItem *item) = 0;
    virtual void setActiveLine(FcConnectLine *line) = 0;

    std::function<void()> geometryChanged;
    std::function<void()> viewChanged;
};