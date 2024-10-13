#pragma once

#include <qobject.h>

#include "../commonnodestaterender.h"

class NodeNormalStateRender : public CommonNodeStateRender {
public:
    explicit NodeNormalStateRender(const QSharedPointer<GraphicObjectData>& data);

protected:
    void drawObject(bool isActiveState) override;

private:
    QSharedPointer<GraphicNodeData> d;
};
