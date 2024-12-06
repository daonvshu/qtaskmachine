#pragma once

#include <qobject.h>

class ViewCenterManager {
public:
    static void beginFile(const QString& fileName);

    static QPointF getViewCenter(const QString& flowName);

    static void setViewCenter(const QString& flowName, const QPointF& center);
};
