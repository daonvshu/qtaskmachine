#pragma once

#include <qobject.h>
#include <qset.h>

#include "../graphiclayer.h"
#include "../graphicobject.h"

#include "../objects/graphiclinkline.h"

/**
 * @brief 当前编辑状态的连线
 */
class ActiveLinkLineLayer : public GraphicLayer {
public:
    explicit ActiveLinkLineLayer(QObject *parent = nullptr);

    void reload(QPainter *painter) override;

    void reCache() override;

public:
    QSet<GraphicLinkLine*> activeLinkLineList;
};
