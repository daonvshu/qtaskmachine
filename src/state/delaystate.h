#pragma once

#include "linearstate.h"

#include <qobject.h>

class DelayState : public LinearState
{
public:
    explicit DelayState(int delayMs, QState *parent = nullptr);

    explicit DelayState(const std::function<int()>& delayReader, QState *parent = nullptr);

    /**
     * @brief 重新设置延时，必须再onEntry之前调用，设置0将立即状态切换
     * @param ms
     */
    void setDelay(int ms);

protected:
    void onEntry(QEvent *event) override;

private:
    int delayMs;
    std::function<int()> delayReader;
};
