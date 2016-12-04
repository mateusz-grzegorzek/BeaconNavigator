#include "device.h"
#include "beacons.h"
#include "logger.h"
#include <QBluetoothLocalDevice>

Device::Device(Beacons* beacons):
    m_device_scan_state(false), m_beacons(beacons){
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    connect(discoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
            this, SLOT(addDevice(const QBluetoothDeviceInfo&)));
    connect(discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
            this, SLOT(deviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(deviceScanFinished()));
    connect(this, SIGNAL(discoveryAgentStartSucces()), m_beacons, SLOT(deviceStarted()));
    connect(this, SIGNAL(discoveryAgentStartError()), m_beacons, SLOT(deviceFailed()));
    qRegisterMetaType<QBluetoothDeviceDiscoveryAgent::Error>("QBluetoothDeviceDiscoveryAgent::Error");
}

void Device::run(){
    Logger::logMessage("Device::run");
    if(!turnOnBluetooth()){
        Logger::logMessage("Device: Bluetooth not available on this device!");
        return;
    }
    m_discovery_agent_started = false;
    m_device_scan_state = true;
    while(m_device_scan_state){
        startDeviceDiscovery();
        QThread::sleep(1);
        stopDeviceDiscovery();
    }
}

bool Device::turnOnBluetooth(){
    QBluetoothLocalDevice localDevice;
    if (localDevice.isValid()) {
        localDevice.powerOn();
        QThread::sleep(1);
        return true;
    } else {
        return false;
    }
}

Device::~Device(){
    delete discoveryAgent;
}

void Device::startDeviceDiscovery(){
    Logger::logMessage("Device::startDeviceDiscovery");
    discoveryAgent->start();
}

void Device::stopDeviceDiscovery(){
    Logger::logMessage("Device::stopDeviceDiscovery");
    discoveryAgent->stop();
}

void Device::turnOff(){
    Logger::logMessage("Device::turnOff");
    m_device_scan_state = false;
}

bool Device::getScanState(){
    return m_device_scan_state;
}

void Device::addDevice(const QBluetoothDeviceInfo &info){
    discoveryAgentStarted();
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration){
        m_beacons->updateBeaconInfo(info.address().toString(), info.rssi());
    }
}

void Device::deviceScanFinished(){
    Logger::logMessage("deviceScanFinished");
    m_device_scan_state = false;
}

void Device::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error){
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError){
        QString msg = "The Bluetooth adapter is powered off, power it on before doing discovery.";
        Logger::logMessage(msg);
    } else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError){
        QString msg = "Writing or reading from the device resulted in an error.";
        Logger::logMessage(msg);
    } else {
        QString msg = "An unknown error has occurred.";
        Logger::logMessage(msg);
    }
    m_device_scan_state = false;
    Q_EMIT discoveryAgentStartError();
}

void Device::discoveryAgentStarted(){
    if(!m_discovery_agent_started){
        m_discovery_agent_started = true;
        Q_EMIT discoveryAgentStartSucces();
    }
}
