#pragma once

#include <qobject.h>

#include "../graphiclayer.h"
#include "../graphicobject.h"

/**
 * @brief 辅助对齐线
 */
class AssistAlignmentLayer : public GraphicLayer {
public:
    explicit AssistAlignmentLayer(QObject* parent = nullptr);

    void reload(QPainter* painter) override;

    void reCache() override;

public:
    QList<QLineF> lines;
};