#ifndef BEACONS_H
#define BEACONS_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QThread>
#include <QMutex>
#include "device.h"
#include "calculator.h"
#include "navigator.h"
#include "point.h"

QT_FORWARD_DECLARE_CLASS (Device)
QT_FORWARD_DECLARE_CLASS (Navigator)

struct DistanceToBeacon
{
    Point point;
    double distance;
};

class Beacons: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString info READ getInfo WRITE setInfo NOTIFY infoChanged)
    Q_PROPERTY(QString position READ getPosition NOTIFY positionChanged)
    Q_PROPERTY(bool state READ state NOTIFY stateChanged)
public:
    Beacons();
    virtual ~Beacons();
    void setDevice(Device* device);
    void setCalculator(Calculator* calculator);
    void setNavigator(Navigator* navigator);
    QString getInfo();
    void setInfo(QString info);
    QString getPosition();
    void updatePosition();
    bool state();
    bool checkMacAddress(QString mac_address);
    void updateDistance(QString mac_address, qint16 rssi);
public slots:
    void startTracking();
    void stopTracking();
    void exitApplication();
Q_SIGNALS:
    void infoChanged();
    void positionChanged();
    void stateChanged();
private:
    void startScan();
    void stopScan();
    void startNavigate();
    void stopNavigate();

    Device* m_device;
    Calculator* m_calculator;
    Navigator* m_navigator;
    QMap<QString, DistanceToBeacon> m_beacons;
    QMutex m_mutex;
    QString m_info;
    QString m_position;

    QString start_tracking_msg = "Start Tracking";
};

#endif // BEACONS_H