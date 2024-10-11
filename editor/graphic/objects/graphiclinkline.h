#pragma once

#include <qobject.h>

#include "graphiclinkline.d.h"

#include "../graphicobject.h"

#include "../render/graphiclinklinerender.h"

class GraphicLinkLine : public GraphicObject {
public:
    explicit GraphicLinkLine(const QSharedPointer<GraphicLinkLineData>& data);

    static QSharedPointer<GraphicLinkLine> create() {
        return QSharedPointer<GraphicLinkLine>::create(QSharedPointer<GraphicLinkLineData>::create());
    }

    bool selectTest(const QPointF &point) override;

    GraphicRenderInterface * getRender() override {
        return new GraphicLinkLineRender(linkData);
    }

    GraphicObjectType objectType() override {
        return GraphicObjectType::Link_Line;
    }

public:
    QSharedPointer<GraphicLinkLineData> linkData;
};

typedef QList<QSharedPointer<GraphicLinkLine>> GraphicLinkLineList;
