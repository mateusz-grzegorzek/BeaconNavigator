#include "tcpserver.h"

void TcpServer::startServer()
{
    m_tcp_server = new QTcpServer();
    connect(m_tcp_server, SIGNAL(newConnection()), this, SLOT(addClient()));

    if(!m_tcp_server->listen(QHostAddress::AnyIPv4, 9999))
    {
        logMessage("Server could not start");
    }
    else
    {
        logMessage("Server started!");
    }
}

void TcpServer::setLogger(Logger *logger)
{
    m_logger = logger;
    connect(m_logger, SIGNAL(newLog()), this, SLOT(sendLogsToClients()));
}

void TcpServer::addClient()
{
    QTcpSocket* socket = m_tcp_server->nextPendingConnection();
    logMessage("Client connected");
    sockets.append(socket);
    socket->write("Hello in Beacon Navigator log terminal.\r\n");
}

void TcpServer::sendLogsToClients()
{
    QList<QString> logs = m_logger->popLogs();
    for(QTcpSocket* socket: sockets)
    {
        for(QString log: logs)
        {
            socket->write(log.toStdString().c_str());
        }
    }
}
