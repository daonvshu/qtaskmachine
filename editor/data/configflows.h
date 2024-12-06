#pragma once

#include <qrect.h>

#include "datakey.h"
#include "basepropertydata.h"

#include "graphic/graphicobjecttype.h"

using namespace QDataUtil;

struct ConfigFlowExecutor : DataDumpInterface {

    DATA_KEY(int, id); //节点id
    DATA_KEY(QString, text); //字符
    DATA_KEY(qint64, taskId); //任务id
    DATA_KEY(qreal, x); //scene位置x
    DATA_KEY(qreal, y); //scene位置y
    DATA_KEY(int, type); //类型 -> Type

    DATA_KEY(QString, enter); //进入函数
    DATA_KEY(QString, exit); //退出函数

    DATA_KEY(QList<PropertyBindData>, properties); //属性绑定

    DATA_KEY(int, delay); //延时
    DATA_KEY(int, timeout); //超时
    DATA_KEY(int, retry); //重试
    DATA_KEY(QString, funcRetry); //重试槽函数
    DATA_KEY(bool, nested); //是否嵌套

    DATA_KEY(QString, condition); //条件检查函数

    GraphicObjectType itemType() const {
        return GraphicObjectType(type());
    }

    void fromType(GraphicObjectType t) {
        type = (int)t;
    }

    QPointF scenePos() const {
        return { x(), y() };
    }

    void fromScenePos(const QPointF& p) {
        x = p.x();
        y = p.y();
    }

    QList<DataReadInterface *> prop() override {
        return { &id, &text, &taskId, &x, &y, &type,
                 &enter, &exit, &properties, &delay, &timeout, &retry, &funcRetry, &nested, &condition };
    }
};

struct ConfigFlowConnectLine : DataDumpInterface {

    DATA_KEY(int, connectFrom); //连接开始节点id
    DATA_KEY(int, connectFromPIndex); //连接开始节点端点index
    DATA_KEY(int, connectTo); //连接结束节点id
    DATA_KEY(int, connectToPIndex); //连接结束节点端点index
    DATA_KEY(qreal, ctlPx); //控制点x
    DATA_KEY(qreal, ctlPy); //控制点y

    DATA_KEY(QString, trigger); //信号触发函数
    DATA_KEY(QString, checkFunc); //检查函数
    DATA_KEY(int, branchId); //分支// id
    DATA_KEY(QString, branchName); //分支名称
    DATA_KEY(bool, failBranch); //失败分支
    DATA_KEY(bool, subBranch); //子流程分支

    QPointF controlPos() const {
        return { ctlPx(), ctlPy() };
    }

    void fromControlPos(const QPointF& p) {
        ctlPx = p.x();
        ctlPy = p.y();
    }

    QList<DataReadInterface *> prop() override {
        return { &connectFrom, &connectFromPIndex, &connectTo, &connectToPIndex, &ctlPx, &ctlPy,
                 &trigger, &checkFunc, &branchId, &branchName, &failBranch, &subBranch };
    }
};

struct ConfigFlow : DataDumpInterface {

    DATA_KEY(QString, name); //流程名
    DATA_KEY(QList<ConfigFlowExecutor>, executors); //节点
    DATA_KEY(QList<ConfigFlowConnectLine>, lines); //连接线

    DATA_KEY(int, version);

    ConfigFlow() {
        version = 1;
    }

    QList<DataReadInterface *> prop() override {
        return { &name, &executors, &lines,
            &version };
    }
};

struct ConfigFlowGroup : DataDumpInterface {

    DATA_KEY(QList<ConfigFlow>, flows);

    QList<DataReadInterface *> prop() override {
        return { &flows };
    }
};