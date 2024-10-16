#pragma once

#include "datakey.h"

using namespace QDataUtil;

struct EventTriggerFunction : DataDumpInterface {
    DATA_KEY(QString, triggerFunc);   // 触发函数
    DATA_KEY(QString, checkFunc);     // 检查函数

    QList<DataReadInterface *> prop() override {
        return {
            &triggerFunc,
            &checkFunc
        };
    }
};

struct EventStatePropertyData : DataDumpInterface {
    DATA_KEY(int, timeout);                      // 超时时间
    DATA_KEY(int, timeoutRetryCount);            // 超时重试次数
    DATA_KEY(QString, timeoutRetryCallback);     // 超时重试回调
    DATA_KEY(EventTriggerFunction, normalEvent); // 正常事件触发信号
    DATA_KEY(EventTriggerFunction, errorEvent);  // 错误事件触发信号

    QList<DataReadInterface *> prop() override {
        return {
            &timeout,
            &timeoutRetryCount,
            &timeoutRetryCallback,
            &normalEvent,
            &errorEvent
        };
    }
};