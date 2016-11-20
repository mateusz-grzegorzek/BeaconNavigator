#include "loggerinterface.h"
#include <QDebug>

void LoggerInterface::setLogger(Logger *logger)
{
    m_logger = logger;
}

void LoggerInterface::logMessage(QString log)
{
    qDebug() << log;
    if(m_logger != 0)
    {
        m_logger->pushLog(log + "\r\n");
    }
}
