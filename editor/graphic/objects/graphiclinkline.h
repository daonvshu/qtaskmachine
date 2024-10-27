#pragma once

#include <qobject.h>

#include "graphiclinkline.d.h"

#include "../graphicobject.h"

#include "../render/graphiclinklinerender.h"

class GraphicLinkLine : public GraphicObject {
public:
    explicit GraphicLinkLine(const QSharedPointer<GraphicLinkLineData>& data);

    static QSharedPointer<GraphicLinkLine> create() {
        return objectCreate<GraphicLinkLine, GraphicLinkLineData>();
    }

    QSharedPointer<GraphicObject> clone() override {
        return objectCreate<GraphicLinkLine, GraphicLinkLineData>(*linkData);
    }

    bool selectTest(const QPointF &point) override;

    GraphicRenderInterface * getRender() override {
        return new GraphicLinkLineRender(linkData);
    }

    GraphicObjectType objectType() const override {
        return GraphicObjectType::Link_Line;
    }

public:
    QSharedPointer<GraphicLinkLineData> linkData;
};

typedef QList<QSharedPointer<GraphicLinkLine>> GraphicLinkLineList;
