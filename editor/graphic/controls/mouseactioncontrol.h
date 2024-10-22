#pragma once

#include <qobject.h>
#include <qevent.h>

#include "../graphiccontrol.h"

class MouseActionControl : public GraphicControl {
public:
    using GraphicControl::GraphicControl;

    /**
     * @brief 鼠标点击
     * @param e
     */
    void mousePress(QMouseEvent* e);

    /**
     * @brief 鼠标移动
     * @param e
     */
    void mouseMove(QMouseEvent* e);

    /**
     * @brief 鼠标释放
     * @param e
     */
    void mouseRelease(QMouseEvent* e);

    /**
     * @brief 鼠标滚轮
     * @param e
     */
    void wheelEvent(QWheelEvent* e);

    /**
     * @brief 右键菜单
     * @param event
     */
    void showContextMenu(QContextMenuEvent *event);

private:
    bool objectSelected = false;
    bool linkLineCreating = false;
    bool linkLineSelected = false;
    QPointF lastMousePoint;
    QSharedPointer<class GraphicNode> lastHoverActiveNode;

private:
    void selectObjPress(const QPoint& mousePos);
    void selectObjMove(const QPoint& mousePos);
    void selectObjRelease();

    void linkLineMove(const QPoint& mousePos);
    void linkLineRelease();

    /**
     * @brief 显示节点的右键菜单
     * @param obj
     * @param event
     */
    void showSelectedObjectMenu(const QSharedPointer<class GraphicObject>& obj, QContextMenuEvent *event);

    /**
     * @brief 显示链接线的右键菜单
     * @param obj
     * @param event
     */
    void showLinkLineMenu(const QSharedPointer<class GraphicObject>& obj, QContextMenuEvent *event);

    /**
     * @brief 显示背景的右键菜单
     * @param event
     */
    void showBlackboardMenu(QContextMenuEvent *event);
};
