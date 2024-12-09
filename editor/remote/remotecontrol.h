#pragma once

#include <qobject.h>
#include <qtcpsocket.h>
#include <protocolcodecengine.h>

#include "protocol.h"

class RemoteControl : public QObject {
    Q_OBJECT

public:
    explicit RemoteControl(QObject* parent = nullptr);
    void connectToDevice(const QString& host, quint16 port);
    void closeConnection();

    FlowStateInfo getFlowState(const QString& flowName);
    QList<ReceiveActiveNode> getActiveNodes(const QString& flowName);
    bool isConnected() const;

    QString getConnectIp() const;
    quint16 getConnectPort() const;

signals:
    void connected();
    void disconnected();
    void socketError(const QString& error);
    void stateRefreshed();
    void receiveNewLog(const ReceiveNewLog& log);
    void receiveActiveNode(const ReceiveActiveNode& activeNode);
    void receiveFlowBegin(const QString& flowName);
    void receiveFlowFinished(const QString& flowName);

private:
    QTcpSocket* tcpSocket;
    QString oldConnectIp;
    quint16 oldConnectPort;
    protocol_codec::ProtocolCodecEngine protocolEngine;
    QHash<QString, FlowStateInfo> currentFlowState;
    QHash<QString, QList<ReceiveActiveNode>> activeNodes;

private:
    void onReceiveFlowState(const ReceiveFlowState& flowState);
    void onReceiveNewLog(const ReceiveNewLog& newLog);
    void onReceiveActiveNode(const ReceiveActiveNode& activeNode);
    void onReceiveFlowBegin(const ReceiveFlowBegin& flowBegin);
    void onReceiveFlowFinished(const ReceiveFlowFinished& flowFinished);
};
