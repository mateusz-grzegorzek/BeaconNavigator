#include "loggerinterface.h"
#include <QDebug>
#include <QTime>

LoggerInterface::LoggerInterface()
{
    m_logger = Q_NULLPTR;
}

void LoggerInterface::setLogger(Logger *logger)
{
    m_logger = logger;
}

void LoggerInterface::logMessage(QString log)
{
    qDebug() << log;
    if(m_logger != Q_NULLPTR)
    {
        m_logger->pushLog(log + "\r\n");
    }
}
