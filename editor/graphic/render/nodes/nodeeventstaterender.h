#pragma once

#include <qobject.h>

#include "../commonnodestaterender.h"

#include "../../objects/nodes/nodeeventstate.d.h"

class NodeEventStateRender : public CommonNodeStateRender {
public:
    explicit NodeEventStateRender(const QSharedPointer<NodeEventStateData>& data);

protected:
    void drawObject(bool isActiveState) override;

private:
    QSharedPointer<NodeEventStateData> d;
};
