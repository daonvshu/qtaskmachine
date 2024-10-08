#pragma once

#include <qobject.h>
#include <qmimedata.h>
#include <qdatastream.h>
#include <qrect.h>

enum class FlowChartNodeType {
    Node_Begin = 0,         //开始状态
    Node_End,               //结束状态
    Node_Normal,            //普通状态
    Node_Delay,             //延时状态
    Node_Event,             //事件触发状态
    Node_MultiEvent,        //复合事件状态
    Node_Condition,         //条件分支状态
    Node_Group,             //分组
    Node_History,           //恢复点
};

inline QString nodeTypeToString(FlowChartNodeType nodeType) {
    switch (nodeType) {
        case FlowChartNodeType::Node_Begin:
            return QStringLiteral("开始状态");
        case FlowChartNodeType::Node_End:
            return QStringLiteral("结束状态");
        case FlowChartNodeType::Node_Normal:
            return QStringLiteral("普通状态");
        case FlowChartNodeType::Node_Delay:
            return QStringLiteral("延时状态");
        case FlowChartNodeType::Node_Event:
            return QStringLiteral("事件触发状态");
        case FlowChartNodeType::Node_MultiEvent:
            return QStringLiteral("复合事件状态");
        case FlowChartNodeType::Node_Condition:
            return QStringLiteral("条件分支状态");
        case FlowChartNodeType::Node_Group:
            return QStringLiteral("子状态组");
        case FlowChartNodeType::Node_History:
            return QStringLiteral("恢复点");
    }
    return {};
}

struct PropertyBindData {
    bool callOnEntered = true; //进入还是退出时调用
    QString key; //属性key
    QString value; //属性值
    QString valueType; //属性值类型
};

struct FlowChartItemData {
    QString text;
    FlowChartNodeType nodeType = FlowChartNodeType::Node_Normal;
    qint64 taskId = -1;
    int flowId = -1; //存储和运行前更新一次

    QString functionEnter; //进入函数
    QString functionExit; //退出函数

    QList<PropertyBindData> properties; //属性绑定列表

    int delayMs = 0; //延时

    int timeoutMs = 0; //超时
    int timeoutRetry = 0; //重试次数
    QString functionRetry; //重试触发槽函数
    bool nested = false; //是否嵌套历史

    FlowChartItemData() = default;
    explicit FlowChartItemData(FlowChartNodeType nodeType)
        : nodeType(nodeType)
    {}
    FlowChartItemData(QString text, qint64 taskId, FlowChartNodeType nodeType)
        : text(std::move(text))
        , taskId(taskId)
        , nodeType(nodeType)
    {}

    bool operator==(const FlowChartItemData& other) const {
        return text == other.text && nodeType == other.nodeType && taskId == other.taskId;
    }
};

Q_DECLARE_METATYPE(FlowChartItemData)

inline QDataStream& operator<<(QDataStream& out, const FlowChartItemData& data) {
    out << data.text << (int)data.nodeType << data.taskId
        << data.functionEnter << data.functionExit
        << data.delayMs
        << data.timeoutMs << data.timeoutRetry << data.functionRetry;
    return out;
}

inline QDataStream& operator>>(QDataStream& in, FlowChartItemData& data) {
    int nodeType;
    in >> data.text >> nodeType >> data.taskId
       >> data.functionEnter >> data.functionExit
       >> data.delayMs
       >> data.timeoutMs >> data.timeoutRetry >> data.functionRetry;
    data.nodeType = FlowChartNodeType(nodeType);
    return in;
}

struct FlowChartConditionData {
    QString text;
    QString functionEnter; //进入函数
    QString functionExit; //退出函数

    QList<PropertyBindData> properties; //属性绑定列表

    QString functionCondition; //条件函数
};

struct FlowChartLineData {
    QString functionTrigger; //信号触发函数
    QString functionCheck; //检查函数
    int branchId = -1; //分支id
    bool failBranch = false; //失败分支
    bool subBranch = false; //子流程分支
};

class FlowChartMimeData : public QMimeData {
public:
    explicit FlowChartMimeData(const FlowChartItemData& itemData);

    static QString mimeType() {
        return "application/flowchart-item-data";
    }

    static FlowChartItemData unpack(const QMimeData* mimeData);

private:
    void setCustomData(const FlowChartItemData& itemData);
};