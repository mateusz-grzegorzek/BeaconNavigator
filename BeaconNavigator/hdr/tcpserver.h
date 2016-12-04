#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include <QMutex>
#include <QThread>

class TcpServer: public QObject{
    Q_OBJECT
public:
    void startServer();

    static TcpServer* s_tcp_server;
private slots:
    void addClient();
    void sendLogToClients(QString log);
private:
    QTcpServer* m_tcp_server;
    QList<QTcpSocket*> m_sockets;
    QMutex m_sockets_mutex;
};

#endif // TCPSERVER_H
