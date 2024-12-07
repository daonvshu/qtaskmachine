#pragma once

#include <qobject.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>

class RemoteDebugListener : public QObject {
public:
    static void startService(int port);

private:
    explicit RemoteDebugListener();

    static RemoteDebugListener& instance();

    void createListener(int port);

    void onNewConnection();

private:
    QTcpServer* tcpServer;
    QList<QTcpSocket*> remoteConnections;
};
