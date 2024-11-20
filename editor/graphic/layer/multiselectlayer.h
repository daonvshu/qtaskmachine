#pragma once

#include <qobject.h>

#include "../graphiclayer.h"
#include "../graphicobject.h"

/**
 * @brief 多选状态的绘制
 */
class MultiSelectLayer : public GraphicLayer {
public:
    explicit MultiSelectLayer(QObject* parent = nullptr);

    void reload(QPainter* painter) override;

    void reCache() override;

    void beginSelect(const QPointF& mousePoint);

    void updateSelect(const QPointF& mousePoint);

    void endSelect();

public:
    QPointF beginPoint;
    QRectF selectRect;
};
