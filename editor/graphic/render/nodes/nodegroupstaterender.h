#pragma once

#include <qobject.h>

#include "../commonnodestaterender.h"

#include "../../objects/nodes/nodestategroup.d.h"

class NodeGroupStateRender : public CommonNodeStateRender {
public:
    explicit NodeGroupStateRender(const QSharedPointer<NodeStateGroupData>& data);

protected:
    void drawObject(bool isActiveState) override;

private:
    QSharedPointer<NodeStateGroupData> d;
};
