#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QMutex>
#include <QFile>

class Logger: public QObject
{
    Q_OBJECT
public:
    Logger(QString path_to_logs);
    QFile* createLogFile(QString file_name);
    bool openLogFile(QFile* file);
    void closeLogFile(QFile* file);
    void saveLog(QFile* file, QString log);
    void pushLog(QString log);
    QList<QString> popLogs();
Q_SIGNALS:
    void newLog();
private:
    QList<QString> m_logs;
    QMutex m_mutex;
    QString m_path_to_logs;
};

#endif // LOGGER_H
