#ifndef BEACONS_H
#define BEACONS_H

#include "distancetobeacon.h"
#include <QObject>
#include <QFile>
#include <QMutex>
#include <QMap>

class Device;
class Navigator;
class Calculator;

enum estimation_type
{
    multilateration,
    weighted_arith_mean
};

class Beacons: public QObject
{
    Q_OBJECT
public:
    Beacons();
    void initModules();

    void startNavigation();
    bool isNavigating();
    void stopNavigation();

    void updateBeaconInfo(QString mac_address, qint16 rssi);
    bool checkMacAddress(const QString& mac_address);

    void changeEstimation();
    estimation_type getEstimationType();

    QList<DistanceToBeacon> getDistances();
    Device *getDevice();
public slots:
    void deviceStarted();
    void deviceFailed();
    void updatePosition(Point position);
Q_SIGNALS:
    void positionChanged(Point);
    void showPosition(bool);
    void navigatorStateChanged(bool);
private:
    void loadBeacons();
    void updateDistance(QString mac_address, qint16 rssi);

    Device* m_device;
    Navigator* m_navigator;

    QMap<QString, DistanceToBeacon> m_beacons;
    QMutex m_beacons_mutex;
    estimation_type m_estimation_type;

    QFile* m_rssi_log_file;
    QFile* m_track_log_file;
};

#endif // BEACONS_H
