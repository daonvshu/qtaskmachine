#pragma once

#include <qobject.h>

#include "graphicnode.d.h"

#include "../graphicobject.h"

#include "data/configflows.h"

class GraphicNode : public GraphicObject {
public:
    explicit GraphicNode(const QSharedPointer<GraphicNodeData>& data);

    bool selectTest(const QPointF& point) override;

    /**
     * @brief 测试是否在链接点上
     * @param point 非GUI坐标点
     * @param testInputPoint 是否在输入点上
     * @return 链接点index，不存在则返回-1
     */
    int testOnLinkPoint(const QPointF& point, bool testInputPoint) const;

    /**
     * @brief 设置链接点是否激活
     * @param linkIndex
     * @param isInputPoint
     */
    void makeLinkPointActive(int linkIndex, bool isInputPoint) const;

    /**
     * @brief 获取链接点颜色
     * @param linkIndex
     * @param isInputPoint
     * @return
     */
    virtual QColor getLinkPointColor(int linkIndex, bool isInputPoint) const;

    virtual ConfigFlowExecutor toFlowExecutor() const;

    virtual void fromExecutor(const ConfigFlowExecutor& executor);

public:
    QSharedPointer<GraphicNodeData> nodeData;
};
