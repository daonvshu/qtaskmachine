#pragma once

#include <qrect.h>

#include "datakey.h"

namespace TaskMachine {
    enum class FlowChartNodeType {
        Node_Begin = 0, //开始状态
        Node_End, //结束状态
        Node_Normal, //普通状态
        Node_Delay, //延时状态
        Node_Event, //事件触发状态
        Node_MultiEvent, //复合事件状态
        Node_Condition, //条件分支状态
        Node_Group, //分组
        Node_History, //恢复点
        Node_Loop, //循环执行状态
    };

    struct ConfigFlowPropertyBind : DataDumpInterface {
        T_DATA_KEY(bool, callOnEntered); //进入还是退出时调用
        T_DATA_KEY(QString, key); //属性key
        T_DATA_KEY(QString, value); //属性值
        T_DATA_KEY(QString, valueType); //属性值类型

        QList<DataReadInterface*> prop() override {
            return {&callOnEntered, &key, &value, &valueType};
        }
    };

    struct ConfigFlowExecutor : DataDumpInterface {
        T_DATA_KEY(int, id); //节点id
        T_DATA_KEY(QString, uuid); //节点id（唯一）
        T_DATA_KEY(QString, text); //字符
        T_DATA_KEY(qint64, taskId); //任务id
        T_DATA_KEY(qreal, x); //scene位置x
        T_DATA_KEY(qreal, y); //scene位置y
        T_DATA_KEY(int, type); //类型 -> Type

        T_DATA_KEY(QString, enter); //进入函数
        T_DATA_KEY(QString, exit); //退出函数

        T_DATA_KEY(bool, printOnEnter); //进入时打印信息
        T_DATA_KEY(bool, printOnExit); //退出时打印信息

        T_DATA_KEY(QList<ConfigFlowPropertyBind>, properties); //属性绑定

        T_DATA_KEY(int, delay); //延时
        T_DATA_KEY(QString, delayProperty); //延时值属性
        T_DATA_KEY(int, timeout); //超时
        T_DATA_KEY(int, retry); //重试
        T_DATA_KEY(QString, funcRetry); //重试槽函数
        T_DATA_KEY(bool, nested); //是否嵌套
        T_DATA_KEY(QString, total); //总次数(属性)

        T_DATA_KEY(QString, condition); //条件检查函数
        T_DATA_KEY(bool, nonBlock); //非阻塞

        ConfigFlowExecutor() {
            printOnEnter = true;
            printOnExit = false;

            nonBlock = true;
        }

        FlowChartNodeType itemType() const {
            return FlowChartNodeType(type());
        }

        void fromType(FlowChartNodeType t) {
            type = (int)t;
        }

        QPointF scenePos() const {
            return {x(), y()};
        }

        void fromScenePos(const QPointF& p) {
            x = p.x();
            y = p.y();
        }

        QList<DataReadInterface*> prop() override {
            return {
                &id, &uuid, &text, &taskId, &x, &y, &type,
                &enter, &exit, &printOnEnter, &printOnExit, &properties,
                &delay, &delayProperty, &timeout, &retry, &funcRetry, &nested, &total,
                &condition, &nonBlock
            };
        }
    };

    struct ConfigFlowConnectLine : DataDumpInterface {
        T_DATA_KEY(int, connectFrom); //连接开始节点id
        T_DATA_KEY(int, connectFromPIndex); //连接开始节点端点index
        T_DATA_KEY(int, connectTo); //连接结束节点id
        T_DATA_KEY(int, connectToPIndex); //连接结束节点端点index
        T_DATA_KEY(qreal, ctlPx); //控制点x
        T_DATA_KEY(qreal, ctlPy); //控制点y

        T_DATA_KEY(QString, trigger); //信号触发函数
        T_DATA_KEY(QString, checkFunc); //检查函数
        T_DATA_KEY(int, branchId); //分支// id
        T_DATA_KEY(QString, branchName); //分支名称
        T_DATA_KEY(bool, failBranch); //失败分支
        T_DATA_KEY(bool, subBranch); //子流程分支

        QPointF controlPos() const {
            return {ctlPx(), ctlPy()};
        }

        void fromControlPos(const QPointF& p) {
            ctlPx = p.x();
            ctlPy = p.y();
        }

        QList<DataReadInterface*> prop() override {
            return {
                &connectFrom, &connectFromPIndex, &connectTo, &connectToPIndex, &ctlPx, &ctlPy,
                &trigger, &checkFunc, &branchId, &branchName, &failBranch, &subBranch
            };
        }
    };

    struct ConfigFlow : DataDumpInterface {
        T_DATA_KEY(QString, name); //流程名
        T_DATA_KEY(QList<ConfigFlowExecutor>, executors); //节点
        T_DATA_KEY(QList<ConfigFlowConnectLine>, lines); //连接线

        qint64 flowId = -1;

        QList<DataReadInterface*> prop() override {
            return {&name, &executors, &lines};
        }
    };

    struct ConfigFlowGroup : DataDumpInterface {
        T_DATA_KEY(QList<ConfigFlow>, flows);

        QList<DataReadInterface*> prop() override {
            return {&flows};
        }
    };
}
