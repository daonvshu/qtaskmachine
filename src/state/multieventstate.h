#pragma once

#include "linearstate.h"
#include "targetsignaltransition.h"

#include <qobject.h>

class MultiEventState : public LinearState {
    Q_OBJECT

public:
    explicit MultiEventState(QState *parent = nullptr);

    /**
     * @brief 添加触发信号，信号和切换的目标状态列表数量必须一致，信号触发之后将切换到对应状态，必须再onEntry之前调用该函数
     * @tparam Func
     * @param sender 信号发送者
     * @param sig 信号函数指针
     */
    template<typename Func>
    void addSignal(const typename QtPrivate::FunctionPointer<Func>::Object *sender, Func sig) {
        signalTransitions << new TargetSignalTransition(sender, sig);
    }

protected:
    void onEntry(QEvent *event) override;

    /**
     * @brief 根据信号返回数据判断是否可以结束信号
     * @param data setSignal中的信号返回的数据列表
     * @return
     */
    virtual bool testFinishBySignalData(const QVariantList& data);

private:
    QList<TargetSignalTransition*> signalTransitions;
};
