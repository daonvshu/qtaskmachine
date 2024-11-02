#pragma once

#include <qobject.h>

#include "commonnodelayer.h"

/**
 * @brief 静态节点图层
 */
class StaticNodeLayer : public CommonNodeLayer {
public:
    explicit StaticNodeLayer(QObject *parent = nullptr);

    void reload(QPainter *painter) override;

    void reCache() override;

public:
    GraphicObjectList staticNodeList;
};
