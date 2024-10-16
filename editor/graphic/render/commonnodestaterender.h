#pragma once

#include <qobject.h>

#include "graphicrender.h"
#include "../objects/graphicnode.d.h"

class CommonNodeStateRender : public GraphicRenderInterface {
public:
    explicit CommonNodeStateRender(const QSharedPointer<GraphicObjectData>& data);

private:
    QSharedPointer<GraphicNodeData> commonData;

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
     * @param isActiveState
     * @return
     */
    QRectF renderNodeBody(int itemRequiredWidth, int itemRequiredHeight, bool isActiveState);

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
};
