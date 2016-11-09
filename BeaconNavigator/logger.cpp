#include "logger.h"
#include "QDebug"

void Logger::pushLog(QString log)
{
    m_mutex.lock();
    m_logs.append(log);
    m_mutex.unlock();
    Q_EMIT newLog();
}

QList<QString> Logger::popLogs()
{
    m_mutex.lock();
    QList<QString> logs_copy = m_logs;
    m_logs.clear();
    m_mutex.unlock();
    return logs_copy;
}
