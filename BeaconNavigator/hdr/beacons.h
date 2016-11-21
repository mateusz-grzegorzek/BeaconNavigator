#ifndef BEACONS_H
#define BEACONS_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QThread>
#include <QMutex>
#include <QFile>
#include "device.h"
#include "calculator.h"
#include "navigator.h"
#include "point.h"
#include "distancetobeacon.h"
#include "logger.h"
#include "loggerinterface.h"

QT_FORWARD_DECLARE_CLASS (Device)
QT_FORWARD_DECLARE_CLASS (Navigator)

enum estimation_type
{
    multilateration,
    weightedArithMean
};

enum mode_type
{
    tracking,
    rssi_measuring
};

class Beacons: public QObject, public LoggerInterface
{
    Q_OBJECT
    Q_PROPERTY(QString info READ getInfo WRITE setInfo NOTIFY infoChanged)
    Q_PROPERTY(QString estimationInfo READ getEstimationInfo NOTIFY estimationInfoChanged)
    Q_PROPERTY(QString position READ getPosition NOTIFY positionChanged)
    Q_PROPERTY(bool trackingState READ trackingState NOTIFY stateChanged)
    Q_PROPERTY(bool rssiMeasState READ rssiMeasState NOTIFY rssiMeasStateChanged)
    Q_PROPERTY(QString rssiMacAddress READ getRssiMacAddress WRITE setRssiMacAddress NOTIFY rssiMacAddressChanged)
public:
    Beacons();
    void setDevice(Device* device);
    void setCalculator(Calculator* calculator);
    void setNavigator(Navigator* navigator);

    QString getInfo();
    QString getEstimationInfo();
    estimation_type getEstimationType();
    void setInfo(QString info);
    void setEstimationInfo(estimation_type type);
    QString getPosition();
    void updatePosition();
    bool trackingState();
    bool rssiMeasState();
    bool checkMacAddress(QString mac_address);
    void updateBeaconInfo(QString mac_address, qint16 rssi);
    double getDistance(QString mac_address);
    QList<DistanceToBeacon> getDistances();
    Device *getDevice();
public slots:
    void startTracking();
    void stopTracking();
    void startMeasuring(QString mac_address);
    void stopMeasuring();
    void editBeacon(QString mac_address, QString position);
    void changeEstimation();
    QString getRssiMacAddress();
    void setRssiMacAddress(QString mac_address);
    void exitApplication();
Q_SIGNALS:
    void infoChanged();
    void estimationInfoChanged();
    void positionChanged();
    void stateChanged();
    void rssiMeasStateChanged();
    void rssiMacAddressChanged();
private:
    void updateDistance(QString mac_address, qint16 rssi);
    void updateRssi(QString mac_address, qint16 rssi);
    bool validateMacAddress(QString mac_address);
    void startScan();
    void stopScan();
    void startNavigate();
    void stopNavigate();
    void logPosition(Point point);

    Device* m_device;
    Calculator* m_calculator;
    Navigator* m_navigator;
    QFile* m_track_log_file;

    QMap<QString, DistanceToBeacon> m_beacons;
    QMutex m_mutex;
    QString m_info;
    QString m_position;
    estimation_type m_estimation_type;

    mode_type m_mode_type;
    bool m_rssi_meas_state;
    QString m_rssi_mac_address;
    QFile* m_rssi_log_file;

    QString start_tracking_msg = "Start Tracking";
};

#endif // BEACONS_H
