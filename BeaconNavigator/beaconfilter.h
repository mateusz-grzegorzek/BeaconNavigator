#ifndef BEACONFILTER_H
#define BEACONFILTER_H

#include <QObject>
#include <QList>

class BeaconFilter: public QObject
{
public:
    BeaconFilter(QList<QString>* beaconsMacAddresses, qint32 minimumRssi);
private:
    QList<QString>* m_beaconsMacAddresses;
    qint32 m_minimumRssi;
};

#endif // BEACONFILTER_H
