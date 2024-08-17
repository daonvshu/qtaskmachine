#pragma once

#include <qapplication.h>

class MyApplication : public QApplication {
public:
    using QApplication::QApplication;

    bool notify(QObject *object, QEvent *event) override;
};
