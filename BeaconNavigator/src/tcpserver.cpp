#include "tcpserver.h"
#include "logger.h"
#include <QDebug>

TcpServer* TcpServer::s_tcp_server;

void TcpServer::startServer(){
    m_tcp_server = new QTcpServer();
    connect(m_tcp_server, SIGNAL(newConnection()), this, SLOT(addClient()));
    connect(Logger::s_logger, SIGNAL(newLog(QString)), this, SLOT(sendLogToClients(QString)));

    if(!m_tcp_server->listen(QHostAddress::AnyIPv4, 9999)){
        qDebug() << "Server could not start";
    } else {
        qDebug() << "Server started!";
    }
}

void TcpServer::addClient(){
    qDebug() << "TcpServer::addClient()";
    QTcpSocket* socket = m_tcp_server->nextPendingConnection();
    m_sockets_mutex.lock();
    m_sockets.append(socket);
    socket->write("Hello in Beacon Navigator log terminal.\r\n");
    m_sockets_mutex.unlock();
}

void TcpServer::sendLogToClients(QString log){
    m_sockets_mutex.lock();
    for(QTcpSocket* socket: m_sockets){
        socket->write(log.toStdString().c_str());
    }
    m_sockets_mutex.unlock();
}
