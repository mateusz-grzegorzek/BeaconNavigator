#include "logger.h"
#include "tcpserver.h"
#include <QDebug>
#include "QDir"
#include <QFile>
#include <QTextStream>
#include <QTime>

QString Logger::m_path_to_logs;
QFile* Logger::m_log_file;
TcpServer* Logger::m_tcp_server;

Logger* Logger::s_logger;

void Logger::emitLog(QString log){
    Q_EMIT newLog(log);
}

void Logger::initLogger(QString path_to_logs){
    s_logger = new Logger;
    TcpServer::s_tcp_server = new TcpServer;
    TcpServer::s_tcp_server->startServer();

    QDir log_dir(path_to_logs);
    QString log_dir_name = "Logs/";
    log_dir.mkdir(log_dir_name);
    m_path_to_logs = path_to_logs + log_dir_name;
    m_log_file = createAndOpenLogFile("log"); //TODO: Display warning if failed
}



void Logger::logMessage(QString log){
    QString log_msg = getTimeStamp();
    log_msg += ": " + log;
    qDebug() << log_msg;
    saveLogToFile(m_log_file, log_msg + "\n");
    s_logger->emitLog(log_msg + "\r\n");
}

QString Logger::getTimeStamp(){
    QTime current_time = QTime::currentTime();
    QString time_stamp = current_time.toString() + ".";
    int msec = current_time.msec();
    if(msec < 100){
        time_stamp += "0";
        if(msec < 10){
            time_stamp += "0";
        }
    }
    time_stamp += QString::number(msec);
    return time_stamp;
}

QFile* Logger::createAndOpenLogFile(QString file_name){
    QString log_file_name = getTimeStamp();
    if(file_name != ""){
        log_file_name +=  "_";
        log_file_name += file_name;
    }
    log_file_name += ".txt";
    QFile* log_file = new QFile(m_path_to_logs + log_file_name);
    if(log_file->open(QIODevice::WriteOnly | QIODevice::Text)){
        return log_file;
    } else {
        return Q_NULLPTR;
    }
}

void Logger::saveLogToFile(QFile* log_file, QString log){
    QTextStream out(log_file);
    out << getTimeStamp() << ": ";
    out << log << "\n";
}

void Logger::closeLogFile(QFile* log_file){
    log_file->close();
}
