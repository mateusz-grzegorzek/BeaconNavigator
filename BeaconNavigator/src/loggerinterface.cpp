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
    QString log_msg = m_logger->getTimeStamp();
    log_msg += ": " + log;
    qDebug() << log_msg;
    if(m_logger != Q_NULLPTR)
    {
        m_logger->pushLog(log_msg + "\r\n");
    }
}
