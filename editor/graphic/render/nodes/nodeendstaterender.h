#pragma once

#include <qobject.h>

#include "../commonnodestaterender.h"

class NodeEndStateRender : public CommonNodeStateRender {
public:
    explicit NodeEndStateRender(const QSharedPointer<GraphicObjectData>& data);

protected:
    void drawObject(bool isActiveState) override;

private:
    QSharedPointer<GraphicNodeData> d;
};
