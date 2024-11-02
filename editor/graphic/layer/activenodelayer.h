#pragma once

#include <qobject.h>

#include "commonnodelayer.h"

/**
 * @brief 编辑中的节点
 */
class ActiveNodeLayer : public CommonNodeLayer {
public:
    explicit ActiveNodeLayer(QObject *parent = nullptr);

    void reload(QPainter *painter) override;

    void reCache() override;

public:
    QSharedPointer<GraphicObject> activeNode;
};
