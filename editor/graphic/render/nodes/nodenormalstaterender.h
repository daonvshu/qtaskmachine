#pragma once

#include <qobject.h>

#include "../graphicrender.h"
#include "../../objects/graphicnode.d.h"

class NodeNormalStateRender : public GraphicRenderInterface {
public:
    explicit NodeNormalStateRender(const QSharedPointer<GraphicObjectData>& data);

protected:
    void drawObject(bool isActiveState) override;

private:
    QSharedPointer<GraphicNodeData> d;
};
