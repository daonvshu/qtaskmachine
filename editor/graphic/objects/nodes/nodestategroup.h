#pragma once

#include <qobject.h>


class NodeStateGroup : public QObject {
public:
    explicit NodeStateGroup(QObject *parent = nullptr);
};
