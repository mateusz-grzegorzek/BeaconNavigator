#ifndef BEACONSMACADDRESSES_H
#define BEACONSMACADDRESSES_H

#include <QString>
#include <QList>

QList<QString>* getBeaconsMacAddresses()
{
    static QList<QString>* beaconsMacAddresses = new QList<QString>;
    beaconsMacAddresses->append("E3:6B:7D:9B:A2:82");
    beaconsMacAddresses->append("F6:C2:B1:B4:11:EC");
    return beaconsMacAddresses;
}

#endif // BEACONSMACADDRESSES_H
