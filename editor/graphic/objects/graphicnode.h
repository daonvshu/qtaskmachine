#pragma once

#include <qobject.h>

#include "graphicnode.d.h"

#include "../graphicobject.h"

#include "data/configflows.h"

class GraphicNode : public GraphicObject {
public:
    explicit GraphicNode(const QSharedPointer<GraphicNodeData>& data);

    bool selectTest(const QPointF& point) const override;

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

    /**
     * @brief 测试链接点是否有效
     * @param linkIndex
     * @param isInputPoint
     * @return
     */
    virtual bool testLinkLineIndexValid(int linkIndex, bool isInputPoint) const;

    virtual ConfigFlowExecutor toFlowExecutor() const;

    virtual void fromExecutor(const ConfigFlowExecutor& executor) const;

protected:
    enum {
        // padding
        titlePadding = 12,
        itemPadding = 16,
        propertyTitlePadding = 4,
        // font size
        titleFontSize = 14,
        itemFontSize = 12,
        propertyTitleFontSize = 12,
        propertyItemFontSize = 12,
        // row height
        titleHeight = 40,
        itemHeight = 31,
        propertyTitleHeight = 23,
        propertyItemHeight = 23,
        splitHeight = 2,
    };

    /**
     * @brief 节点名称宽度
     * @return
     */
    int nodeNameWidth() const;

    /**
     * @brief 进入函数文本宽度
     * @return
     */
    int functionEnterWidth() const;

    /**
     * @brief 退出函数文本宽度
     * @return
     */
    int functionExitWidth() const;

    /**
     * @brief 最小属性设置文本宽度
     * @param [out] bindStrings
     * @return
     */
    int minPropertyWidth(QStringList& bindStrings) const;

    /**
     * @brief 绘制节点背景
     * @param itemRequiredWidth
     * @param itemRequiredHeight
     * @return
     */
    QRectF renderNodeBody(int itemRequiredWidth, int itemRequiredHeight);

    /**
     * @brief 绘制节点标题
     * @param bodyRect
     */
    void renderNodeTitle(const QRectF& bodyRect);

    /**
     * @brief 绘制属性项
     * @param bodyRect
     * @param beginY
     * @param bindStrings
     */
    void renderPropertyItems(const QRectF& bodyRect, qreal beginY, const QStringList& bindStrings);

public:
    QSharedPointer<GraphicNodeData> nodeData;
};
