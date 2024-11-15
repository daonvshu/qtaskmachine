#pragma once

#include <qobject.h>

#include "graphiclinkline.d.h"

#include "../graphicobject.h"

class GraphicLinkLine : public GraphicObject {
public:
    explicit GraphicLinkLine(const QSharedPointer<GraphicLinkLineData>& data);

    static GraphicLinkLine* create() {
        return objectCreate<GraphicLinkLine, GraphicLinkLineData>();
    }

    GraphicObject* clone() const override {
        return objectCreate<GraphicLinkLine, GraphicLinkLineData>(*linkData);
    }

    bool selectTest(const QPointF &point) const override;

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Link_Line;
    }

    void drawObject() override;

public:
    QSharedPointer<GraphicLinkLineData> linkData;
};

typedef QList<QSharedPointer<GraphicLinkLine>> GraphicLinkLineList;
