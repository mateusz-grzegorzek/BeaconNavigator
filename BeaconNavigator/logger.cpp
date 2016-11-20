#include "logger.h"
#include "QDebug"
#include "QDir"
#include <QFile>
#include <QTextStream>
#include <QTime>

Logger::Logger(QString path_to_logs)
{
    QDir log_dir(path_to_logs);
    QString log_dir_name = "Logs/";
    log_dir.mkdir(log_dir_name);
    m_path_to_logs = path_to_logs + log_dir_name;
}

bool Logger::openLogFile(QFile* file)
{
    return file->open(QIODevice::WriteOnly | QIODevice::Text);
}

void Logger::closeLogFile(QFile* file)
{
    file->close();
}

QFile* Logger::createLogFile(QString file_name)
{
    QTime current_time = QTime::currentTime();
    file_name += current_time.toString() + "." + QString::number(current_time.msec());
    file_name += ".txt";
    return new QFile(m_path_to_logs + file_name);
}

void Logger::saveLog(QFile* file, QString log)
{
    QTextStream out(file);
    out << log;
}

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
