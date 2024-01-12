#pragma once

#include <qobject.h>

class PageLoadInterface : public QObject {
public:
    using QObject::QObject;

    virtual QString getDescription() = 0;
    virtual QString getCodeFile() = 0;
    virtual void run() = 0;
};