#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QThread>

class QBluetoothDeviceInfo;
class Beacons;

class Device: public QThread{
    Q_OBJECT
public:
    Device(Beacons* beacons);
    void run();
    ~Device();
    void startDeviceDiscovery();
    void stopDeviceDiscovery();
    void turnOff();
    bool getScanState();
Q_SIGNALS:
    void discoveryAgentStartSucces();
    void discoveryAgentStartError();
private slots:
    void addDevice(const QBluetoothDeviceInfo&);
    void deviceScanFinished();
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error);
private:
    bool turnOnBluetooth();
    void discoveryAgentStarted();

    bool m_discovery_agent_started;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    bool m_device_scan_state;
    Beacons* m_beacons;
};

#endif // DEVICE_H
