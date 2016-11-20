#ifndef LOGGERINTERFACE_H
#define LOGGERINTERFACE_H

#include <QString>
#include "logger.h"

class LoggerInterface
{
public:
    virtual void setLogger(Logger* logger);
    void logMessage(QString log);
    Logger* m_logger;
};

#endif // LOGGERINTERFACE_H
