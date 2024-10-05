#pragma once

#include <qobject.h>

#include "../graphiclayer.h"
#include "../graphicobject.h"

/**
 * @brief 编辑中的节点
 */
class ActiveNodeLayer : public GraphicLayer {
public:
    explicit ActiveNodeLayer(QObject *parent = nullptr);

    void reCache() override;

public:
    QSharedPointer<GraphicObject> activeNode;
};
