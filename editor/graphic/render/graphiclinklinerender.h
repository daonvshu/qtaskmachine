#pragma once

#include <qobject.h>

#include "graphicrender.h"
#include "../objects/graphiclinkline.d.h"

class GraphicLinkLineRender : public GraphicRenderInterface {
public:
    explicit GraphicLinkLineRender(const QSharedPointer<GraphicLinkLineData>& data);

protected:
    void drawObject(bool isActiveState) override;

private:
    QSharedPointer<GraphicLinkLineData> d;
};
