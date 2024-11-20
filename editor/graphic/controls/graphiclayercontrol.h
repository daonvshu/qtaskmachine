#pragma once

#include <qobject.h>

#include "../graphiccontrol.h"
#include "../graphicobject.h"

#include "../objects/graphiclinkline.h"

enum class GraphicLayerType {
    Layer_None =         0,

    Layer_Grid =         1 << 0,   //网格线
    Layer_Static_Node  = 1 << 1,   //静态节点
    Layer_Static_Link  = 1 << 2,   //静态连线
    Layer_Active_Node  = 1 << 3,   //动态节点
    Layer_Active_Link  = 1 << 4,   //动态连线
    Layer_Multi_Select = 1 << 5,   //多选状态

    Layer_All = Layer_Grid | Layer_Static_Node | Layer_Static_Link | Layer_Active_Node | Layer_Active_Link | Layer_Multi_Select,
};
Q_DECLARE_FLAGS(GraphicLayerTypes, GraphicLayerType)
Q_DECLARE_OPERATORS_FOR_FLAGS(GraphicLayerTypes)

class GraphicLayerControl : public GraphicControl {
public:
    explicit GraphicLayerControl(const QSharedPointer<GraphicControlSharedData>& data, QObject *parent = nullptr);

    /**
     * @brief 重置缓冲层
     */
    void graphLayerReload();

    /**
     * @brief 绘制缓冲层
     * @param painter
     */
    void graphLayerRepaint(QPainter* painter);

    /**
     * @brief 是否启用网格线
     * @param enable
     */
    void gridEnable(bool enable);

    /**
     * @brief 刷新指定图层
     * @param layerType
     */
    void reloadLayer(GraphicLayerTypes layerType);

    /**
     * @brief 清空所有图形
     */
    void clearAllGraphic();

    /**
     * @brief 设置当前编辑节点
     * @param activeNode
     */
    void setActiveNode(const GraphicObject* activeNode);

    /**
     * @brief 设置当前编辑连线
     * @param activeLinkLine
     */
    void setActiveLinkLine(GraphicLinkLine* activeLinkLine);

    /**
     * @brief 取消编辑连线
     * @param activeLinkLine
     */
    void cancelActiveLinkLine(const GraphicLinkLine* activeLinkLine);

    /**
     * @brief 取消所有编辑连线
     */
    void cancelAllActiveLinkLine();

    /**
     * @brief 强制更新所有node
     */
    void makeAllStaticNodeChanged() const;

    /**
     * @brief 多选开始
     * @param mousePoint
     */
    void beginMultiSelect(const QPointF& mousePoint);

    /**
     * @brief 多选更新
     * @param mousePoint
     */
    void updateMultiSelect(const QPointF& mousePoint);

    /**
     * @brief 多选结束
     */
    void endMultiSelect();

    /**
     * @brief 获取多选区域
     * @return
     */
    QRectF getMultiSelectRect() const;

    /**
     * @brief 通知多选对象改变
     */
    void multiSelectObjectsChanged();

private:
    QList<QPair<GraphicLayerType, class GraphicLayer*>> layers; //绘制缓冲层
    GraphicLayerTypes graphicResetOption = GraphicLayerType::Layer_None; //重置缓存

private:
    template <typename T>
    T* layer(GraphicLayerType option) const {
        for (const auto& i : layers) {
            if (i.first == option) {
                return dynamic_cast<T*>(i.second);
            }
        }
        return nullptr;
    }
};
