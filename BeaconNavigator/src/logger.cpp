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

QString Logger::getTimeStamp()
{
    QTime current_time = QTime::currentTime();
    QString time_stamp = current_time.toString() + ".";
    int msec = current_time.msec();
    if(msec < 100)
    {
        time_stamp += "0";
        if(msec < 10)
        {
            time_stamp += "0";
        }
    }
    time_stamp += QString::number(msec);
    return time_stamp;
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
    QString log_file_name = getTimeStamp();
    if(file_name != "")
    {
        log_file_name +=  "_";
        log_file_name += file_name;
    }
    log_file_name += ".txt";
    return new QFile(m_path_to_logs + log_file_name);
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
