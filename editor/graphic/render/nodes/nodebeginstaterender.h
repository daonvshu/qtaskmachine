#pragma once

#include <qobject.h>

#include "../commonnodestaterender.h"

class NodeBeginStateRender : public CommonNodeStateRender {
public:
    explicit NodeBeginStateRender(const QSharedPointer<GraphicObjectData>& data);

protected:
    void drawObject(bool isActiveState) override;

private:
    QSharedPointer<GraphicNodeData> d;
};
