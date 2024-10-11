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

    void reCache() override;

public:
    QSet<QSharedPointer<GraphicLinkLine>> activeLinkLineList;
};
