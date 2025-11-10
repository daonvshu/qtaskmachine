#include "remotecontrol.h"

#include <qdebug.h>
#include <qnetworkproxy.h>

PROTOCOL_CODEC_USING_NAMESPACE

RemoteControl::RemoteControl(QObject* parent)
    : QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    tcpSocket->setProxy(QNetworkProxy::NoProxy);
    connect(tcpSocket, &QTcpSocket::connected, this, [&] {
        emit connected();
        currentFlowState.clear();
        tcpSocket->write(protocolEngine.encode<GetFlowStateRequest>());
    });

    connect(tcpSocket, &QTcpSocket::disconnected, this, [&] {
        emit disconnected();
    });

    connect(tcpSocket, &QTcpSocket::errorOccurred, this, [&](QAbstractSocket::SocketError error) {
        QString errorString;
        QDebug(&errorString) << error;
        emit socketError(errorString);
        tcpSocket->close();
    });

    connect(tcpSocket, &QTcpSocket::readyRead, this, [&] {
        protocolEngine.appendBuffer(tcpSocket->readAll());
    });

    protocolEngine.frameDeclare("H(7E3E3F)S2T2CV(CRC16)E(FE)");
    protocolEngine.setVerifyFlags("STC");
    protocolEngine.registerType<CompressCodec<ReceiveFlowState, JsonCodec>>(this, &RemoteControl::onReceiveFlowState);
    protocolEngine.registerType<JsonCodec<ReceiveNewLog>>(this, &RemoteControl::onReceiveNewLog);
    protocolEngine.registerType<JsonCodec<ReceiveActiveNode>>(this, &RemoteControl::onReceiveActiveNode);
    protocolEngine.registerType<JsonCodec<ReceiveFlowBegin>>(this, &RemoteControl::onReceiveFlowBegin);
    protocolEngine.registerType<JsonCodec<ReceiveFlowFinished>>(this, &RemoteControl::onReceiveFlowFinished);

    qRegisterMetaType<ReceiveNewLog>();
    qRegisterMetaType<ReceiveActiveNode>();
}

void RemoteControl::connectToDevice(const QString &host, quint16 port) {
    tcpSocket->connectToHost(host, port);
    oldConnectIp = host;
    oldConnectPort = port;
}

void RemoteControl::closeConnection() {
    tcpSocket->close();
}

FlowStateInfo RemoteControl::getFlowState(const QString &flowName) {
    return currentFlowState[flowName];
}

QList<ReceiveActiveNode> RemoteControl::getActiveNodes(const QString &flowName) {
    return activeNodes[flowName];
}

bool RemoteControl::isConnected() const {
    return tcpSocket->isOpen();
}

QString RemoteControl::getConnectIp() const {
    return oldConnectIp;
}

quint16 RemoteControl::getConnectPort() const {
    return oldConnectPort;
}

void RemoteControl::onReceiveFlowState(const ReceiveFlowState &flowState) {
    for (const auto& state : flowState.flowStates()) {
        currentFlowState[state.flowName()] = state;
    }
    emit stateRefreshed();
}

void RemoteControl::onReceiveNewLog(const ReceiveNewLog &newLog) {
    currentFlowState[newLog.flowName()].logs() << newLog.log();
    emit receiveNewLog(newLog);
}

void RemoteControl::onReceiveActiveNode(const ReceiveActiveNode &activeNode) {
    activeNodes[activeNode.flowName()] << activeNode;
    currentFlowState[activeNode.flowName()].currentRunId = activeNode.nodeId();
    emit receiveActiveNode(activeNode);
}

void RemoteControl::onReceiveFlowBegin(const ReceiveFlowBegin &flowBegin) {
    currentFlowState.remove(flowBegin.flowName());
    activeNodes.remove(flowBegin.flowName());
    emit receiveFlowBegin(flowBegin.flowName());
}

void RemoteControl::onReceiveFlowFinished(const ReceiveFlowFinished &flowFinished) {
    emit receiveFlowFinished(flowFinished.flowName());
}

