#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QMutex>

class Logger: public QObject
{
    Q_OBJECT
public:
    void pushLog(QString);
    QList<QString> popLogs();
Q_SIGNALS:
    void newLog();
private:
    QList<QString> m_logs;
    QMutex m_mutex;
};

#endif // LOGGER_H
