#include <QDebug>
#include "beaconfilter.h"
#include "deviceinfo.h"

BeaconFilter::BeaconFilter(QList<QString>* beaconsMacAddresses, qint32 minimumRssi)
{
    qDebug() << "BeaconFilter";
    qDebug() << "beaconsMacAddresses: ";
    QList<QString>::iterator iMac;
    for (iMac = beaconsMacAddresses->begin(); iMac != beaconsMacAddresses->end(); ++iMac)
    {
        qDebug() << *iMac;
    }
    qDebug() << "minimumRssi: " << minimumRssi;
    m_beaconsMacAddresses = beaconsMacAddresses;
    m_minimumRssi = minimumRssi;
}

BeaconFilter::~BeaconFilter()
{
    delete m_beaconsMacAddresses;
}

QList<QObject *> BeaconFilter::filterBeaconsByMacAddresses(QList<QObject *> beacons)
{
    qDebug() << "filterBeaconsByMacAddresses";

    QMutableListIterator<QObject *> iBeacon(beacons);
    QList<QString>::iterator iMac;
    while (iBeacon.hasNext())
    {
        bool toDelete = true;
        DeviceInfo* di = dynamic_cast<DeviceInfo*>(iBeacon.next());
        for (iMac = m_beaconsMacAddresses->begin(); iMac != m_beaconsMacAddresses->end(); ++iMac)
        {
            if(*iMac == di->getAddress())
            {
                toDelete = false;
                break;
            }
        }
        if(toDelete)
            iBeacon.remove();
    }
    return beacons;
}

QList<QObject *> BeaconFilter::filterBeaconsByRssi(QList<QObject *> beacons)
{
    qDebug() << "filterBeaconsByRssi";

    QMutableListIterator<QObject *> iBeacon(beacons);
    while (iBeacon.hasNext())
    {
        DeviceInfo* di = dynamic_cast<DeviceInfo*>(iBeacon.next());
        if(di->getRssi() < m_minimumRssi)
            iBeacon.remove();
    }
    return beacons;
}
