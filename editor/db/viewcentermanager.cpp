#include "viewcentermanager.h"

#include "viewcenter.h"
#include <dao.h>

static QString currentLoadFile;
void ViewCenterManager::beginFile(const QString& fileName) {
    currentLoadFile = fileName;
}

QPointF ViewCenterManager::getViewCenter(const QString& flowName) {
    Q_ASSERT(!currentLoadFile.isEmpty());
    ViewCenter::Fields vf;
    auto data = dao::_select<ViewCenter>()
        .filter(vf.configPath = currentLoadFile, vf.flowName = flowName)
        .build().unique();
    return { data.x, data.y };
}

void ViewCenterManager::setViewCenter(const QString& flowName, const QPointF& center) {
    Q_ASSERT(!currentLoadFile.isEmpty());
    ViewCenter::Fields vf;
    dao::_insertOrUpdate<ViewCenter>()
        .set(vf.configPath = currentLoadFile, vf.flowName = flowName, vf.x = center.x(), vf.y = center.y())
        .conflictColumns(vf.configPath, vf.flowName)
        .build().insert();
}
