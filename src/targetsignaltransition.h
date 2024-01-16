#pragma once

#include <qobject.h>
#include <qsignaltransition.h>
#include <functional>

class TargetSignalTransition : public QSignalTransition
{
public:
    using QSignalTransition::QSignalTransition;

    void setSignalDataHandler(const std::function<bool(const QVariantList&)>& handler);

protected:
    bool eventTest(QEvent *event) override;

private:
    std::function<bool(const QVariantList&)> signalTester = nullptr;
};
