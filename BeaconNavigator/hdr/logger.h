#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QMutex>
#include <QFile>

class TcpServer;

class Logger: public QObject{
    Q_OBJECT
public:
    void emitLog(QString log);

    static void initLogger(QString path_to_logs);
    static void logMessage(QString log);
    static QString getTimeStamp();

    static QFile* createAndOpenLogFile(QString file_name);
    static void saveLogToFile(QFile* log_file, QString log);
    static void closeLogFile(QFile* log_file);

    static Logger* s_logger;
Q_SIGNALS:
    void newLog(QString);
private:
    static QString m_path_to_logs;
    static QFile* m_log_file;
    static TcpServer* m_tcp_server;
};

#endif // LOGGER_H
