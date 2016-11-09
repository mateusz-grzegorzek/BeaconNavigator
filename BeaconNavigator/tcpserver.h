#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include <QObject>
#include <QThread>
#include "logger.h"
#include "loggerinterface.h"

class TcpServer: public QObject, public LoggerInterface
{
    Q_OBJECT
public:
    void startServer();
    void sendLog(QString log);
    virtual void setLogger(Logger* logger);
private slots:
    void addClient();
    void sendLogsToClients();
private:
    QTcpServer* m_tcp_server;
    QList<QTcpSocket*> sockets;
};

#endif // TCPSERVER_H
