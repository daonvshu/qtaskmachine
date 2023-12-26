#pragma once

#include <qobject.h>
#include <qsignaltransition.h>
#include <functional>

class TargetSignalTransition : public QSignalTransition
{
    Q_OBJECT
public:
    using QSignalTransition::QSignalTransition;

    void setSignalTriggeredSize(int size);

signals:
    void signalTriggered(const QList<QVariant>& arguments);

protected:
    bool eventTest(QEvent *event) override;

private:
    int signalTriggeredSize = 0;
};
