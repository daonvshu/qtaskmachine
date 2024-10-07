#pragma once

#include <qobject.h>

#include "../graphiccontrol.h"
#include "../graphicobject.h"
#include "../graphicobjecttype.h"

class GraphicObjCreateControl : public GraphicControl {
public:
    explicit GraphicObjCreateControl(const QSharedPointer<GraphicControlSharedData>& data, QObject *parent = nullptr);

    void addObject(GraphicObjectType type, const QPoint& mousePoint);

    /**
     * @brief 测试鼠标位置（GUI位置）是否可选中对象
     * @param mousePoint
     * @return
     */
    QSharedPointer<GraphicObject> selectTest(const QPoint& mousePoint);

    /**
     * @brief 平移选中对象
     * @param delta
     */
    void translate(const QPointF& delta);

private:
    GraphicObjectList graphicObjects;
    QSharedPointer<GraphicObject> activeObject;

private:
    void cancelActiveSelected();
};
