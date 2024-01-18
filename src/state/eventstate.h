#pragma once

#include "linearstate.h"
#include "targetsignaltransition.h"

#include <qobject.h>

class EventState : public LinearState {
    Q_OBJECT

public:
    explicit EventState(QState *parent = nullptr);

    /**
     * @brief 添加触发信号，信号触发之后将切换到下一个状态，必须再onEntry之前调用该函数
     * @tparam Func
     * @param sender 信号发送者
     * @param sig 信号函数指针
     */
    template<typename Func>
    void setSignal(const typename QtPrivate::FunctionPointer<Func>::Object *sender, Func sig) {
        signalTransition = new TargetSignalTransition(sender, sig);
    }

    /**
     * @brief 设置超时时间
     * @param ms
     */
    void setTimeout(int ms);

    /**
     * @brief 设置重试次数
     * @param count
     */
    void setRetrySize(int count);

    /**
     * @brief 设置失败切换状态，超时或者主动触发失败
     * @param state
     */
    void setFailState(QAbstractState* state);

signals:
    /**
     * @brief 主动触发失败
     */
    void triggerSignalFail();

    /**
     * @brief 重试信号触发
     */
    void stateRetry(QPrivateSignal);

protected:
    void onEntry(QEvent *event) override;
    void onExit(QEvent *event) override;

    /**
     * @brief 根据信号返回数据判断是否可以结束信号
     * @param data setSignal中的信号返回的数据列表
     * @return
     */
    virtual bool testFinishBySignalData(const QVariantList& data);

    /**
     * @brief 超时处理函数
     */
    virtual void timeoutHandler();

    /**
     * @brief 重置超时检查
     */
    void resetTimeoutCheck();

private:
    TargetSignalTransition* signalTransition;
    QTimer* timeoutCheckTimer;
    QAbstractState* failState;

    int retryCount;
    int curRetrySize;
    int timeoutMs;

    bool transferBySelf;
};
