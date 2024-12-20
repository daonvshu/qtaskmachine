#pragma once

#include <qobject.h>

#include "../graphiclayer.h"
#include "../graphicobject.h"

#include "../objects/graphiclinkline.h"

/**
 * @brief 静态链接线层
 */
class StaticLinkLineLayer : public GraphicLayer {
public:
    explicit StaticLinkLineLayer(QObject *parent = nullptr);

    void reload(QPainter *painter) override;

    void reCache() override;

public:
    QUndoStack* graphicEntries = nullptr;
};
