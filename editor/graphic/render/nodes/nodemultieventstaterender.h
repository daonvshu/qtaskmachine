#pragma once

#include <qobject.h>

#include "../commonnodestaterender.h"

#include "../../objects/nodes/nodemultieventstate.d.h"

class NodeMultiEventStateRender : public CommonNodeStateRender {
public:
    explicit NodeMultiEventStateRender(const QSharedPointer<NodeMultiEventStateData>& data);

protected:
    void drawObject(bool isActiveState) override;

private:
    QSharedPointer<NodeMultiEventStateData> d;
};
