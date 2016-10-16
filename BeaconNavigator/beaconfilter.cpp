#include <QDebug>
#include "beaconfilter.h"

BeaconFilter::BeaconFilter(QList<QString>* beaconsMacAddresses, qint32 minimumRssi)
{
    qDebug() << "BeaconFilter";
    qDebug() << "beaconsMacAddresses: ";
    QList<QString>::iterator i;
    for (i = beaconsMacAddresses->begin(); i != beaconsMacAddresses->end(); ++i)
    {
        qDebug() << *i;
    }
    qDebug() << "minimumRssi: " << minimumRssi;
    m_beaconsMacAddresses = beaconsMacAddresses;
    m_minimumRssi = minimumRssi;
}
