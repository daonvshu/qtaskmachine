#pragma once

#include <qobject.h>


class FlowStateRunner : public QObject {
public:
    explicit FlowStateRunner(QObject *parent = nullptr);
};
