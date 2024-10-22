#pragma once

#include <qobject.h>

#include "../graphiccontrol.h"
#include "../graphicobject.h"
#include "../graphicobjecttype.h"

#include "../objects/graphiclinkline.h"

class GraphicObjCreateControl : public GraphicControl {
public:
    explicit GraphicObjCreateControl(const QSharedPointer<GraphicControlSharedData>& data, QObject *parent = nullptr);

    /**
     * @brief 鼠标位置添加对象
     * @param type
     * @param mousePoint
     */
    void addObject(GraphicObjectType type, const QPoint& mousePoint);

    /**
     * @brief 复制节点到鼠标位置
     * @param nodeObject
     * @param mousePoint
     */
    void copyNodeToMousePoint(const QSharedPointer<GraphicObject>& nodeObject, const QPoint& mousePoint);

    /**
     * @brief 测试鼠标位置（GUI位置）是否可选中对象
     * @param mousePoint
     * @param testSelectedObject
     * @return
     */
    QSharedPointer<GraphicObject> selectTest(const QPoint& mousePoint, bool testSelectedObject = false);

    /**
     * @brief 将指定对象选中
     * @param object
     */
    void setObjectSelected(const QSharedPointer<GraphicObject>& object);

    /**
     * @brief 平移选中对象
     * @param delta
     */
    void objTranslate(const QPointF& delta);

    /**
     * @brief 删除指定节点对象
     * @param object
     */
    void removeNodeObject(const QSharedPointer<GraphicObject>& object);

    /**
     * @brief 取消节点选中
     */
    void cancelObjActiveSelected();

    /**
     * @brief 获取当前选中的节点对象
     * @return
     */
    QSharedPointer<GraphicObject> getSelectedNodeObj();

    /**
     * @brief 创建链接线
     * @param linkFrom
     * @param linkPointIndex
     * @param curMousePoint
     */
    void beginActiveLinkLine(const QSharedPointer<GraphicObject>& linkFrom, int linkPointIndex, const QPointF& curMousePoint);

    /**
     * @brief 更新链接线终点位置
     * @param linkTo
     * @param linkPointIndex
     * @param curMousePoint
     */
    void updateActiveLinkLineToPoint(const QSharedPointer<GraphicObject>& linkTo, int linkPointIndex, const QPointF& curMousePoint);

    /**
     * @brief 释放当前编辑链接线
     */
    void releaseActiveLinkLine();

    /**
     * @brief 设置链接线选中
     * @param object
     */
    void setLinkLineSelected(const QSharedPointer<GraphicObject>& object);

    /**
     * @brief 取消当前选中的线
     */
    void cancelSelectedLinkLine();

    /**
     * @brief 删除指定链接线
     * @param linkLine
     */
    void removeLinkLine(const QSharedPointer<GraphicObject>& linkLine);

    /**
     *  获取当前选中的链接线
     * @return
     */
    QSharedPointer<GraphicLinkLine> getSelectedLinkLine();

    /**
     * @brief 检查当前节点是否被其他链接线连接
     * @param node
     * @param linkIndex
     * @return
     */
    bool checkIsAnyLinkLineLinkedToNode(const QSharedPointer<GraphicNode>& node, int linkIndex);

private:
    GraphicObjectList nodeObjects;
    GraphicLinkLineList linkLines;
    QSharedPointer<GraphicObject> editingNodeObject;
    QSharedPointer<GraphicLinkLine> editingLinkLine;
    QSharedPointer<GraphicLinkLine> selectedLinkLine;
};
