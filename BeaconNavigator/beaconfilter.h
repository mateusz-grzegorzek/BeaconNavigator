#ifndef BEACONFILTER_H
#define BEACONFILTER_H

#include <QObject>
#include <QList>
#include "device.h"

class BeaconFilter: public QObject
{
public:
    BeaconFilter(QList<QString>* beaconsMacAddresses, qint32 minimumRssi);
    QList<QObject *> filterBeaconsByMacAddresses(QList<QObject *> beacons);
    QList<QObject *> filterBeaconsByRssi(QList<QObject *> beacons);
    ~BeaconFilter();

private:
    QList<QString>* m_beaconsMacAddresses;
    qint32 m_minimumRssi;
};

#endif // BEACONFILTER_H
