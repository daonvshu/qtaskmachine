#pragma once

#include <qobject.h>

#include "../graphiclayer.h"
#include "../graphicobject.h"

/**
 * @brief 静态节点图层
 */
class StaticNodeLayer : public GraphicLayer {
public:
    explicit StaticNodeLayer(QObject *parent = nullptr);

    void reCache() override;

public:
    GraphicObjectList staticNodeList;
};
