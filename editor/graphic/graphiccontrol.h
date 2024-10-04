#pragma once

#include <qobject.h>
#include <qsharedpointer.h>
#include <qtransform.h>

#include "graphicview.h"

#include "utils/transform.h"

class GraphicControl;
struct GraphicControlSharedData {
    GraphicView* view = nullptr;

    QTransform coordinateTransform; //坐标轴变换
    QTransform graphicTransform; //坐标变换

    GraphicTransform getGraphicTransform() const {
        return GraphicTransform(coordinateTransform * graphicTransform);
    }

    QList<GraphicControl*> controls;

    template<typename T>
    T* getControl() {
        for (auto control : controls) {
            if (auto ptr = dynamic_cast<T*>(control)) {
                return ptr;
            }
        }
        return nullptr;
    }
};

class GraphicControl : public QObject {
public:
    explicit GraphicControl(const QSharedPointer<GraphicControlSharedData>& data, QObject* parent = nullptr)
        : QObject(parent)
        , d(data)
    {}

protected:
    QSharedPointer<GraphicControlSharedData> d;
};

class GraphicControlWrapper : public GraphicControl {
public:
    explicit GraphicControlWrapper(GraphicView* view, QObject* parent = nullptr)
        : GraphicControl(QSharedPointer<GraphicControlSharedData>::create(), parent)
    {
        d->view = view;
    }

    template<typename T>
    void add() {
        d->controls.append(new T(d, this));
    }

    template<typename T>
    T* get() {
        return d->getControl<T>();
    }
};