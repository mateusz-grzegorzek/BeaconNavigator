#include "beacons.h"
#include <QThread>
#include <QCoreApplication>
#include <QDebug>
#include "tcpserver.h"
#include <QTime>

Beacons::Beacons()
{
    setInfo(start_tracking_msg);
    m_beacons.insert("F6:C2:B1:B4:11:EC", {{0,0}, 0});
    m_beacons.insert("E3:6B:7D:9B:A2:82", {{3,2}, 0});

    // Fake beacons
    m_beacons.insert("E3:6B:7D:9B:A2:83", {{-4,2}, 1.5});
    m_beacons.insert("E3:6B:7D:9B:A2:84", {{-1,-2}, 2.3});
    m_beacons.insert("E3:6B:7D:9B:A2:85", {{-5,1}, 3.1});
}

void Beacons::setDevice(Device *device)
{
    m_device = device;
}

void Beacons::setCalculator(Calculator *calculator)
{
    m_calculator = calculator;
}

void Beacons::setNavigator(Navigator *navigator)
{
    m_navigator = navigator;
}

void Beacons::startScan()
{
    logMessage("Beacons::startScan");
    m_device->start();
}

void Beacons::stopScan()
{
    logMessage("Beacons::stopScan");
    m_device->turnOff();
    m_device->quit();
    m_device->wait();
}

void Beacons::startNavigate()
{
    logMessage("Beacons::startNavigate");
    m_navigator->start();
}

void Beacons::stopNavigate()
{
    logMessage("Beacons::stopNavigate");
    m_navigator->turnOff();
    m_navigator->quit();
    m_navigator->wait();
}

void Beacons::exitApplication()
{
    QCoreApplication::quit();
}

QString Beacons::getInfo()
{
    return m_info;
}

void Beacons::setInfo(QString info)
{
    m_info = info;
    Q_EMIT infoChanged();
}

QString Beacons::getPosition()
{
    return m_position;
}

void Beacons::updatePosition()
{
    Point position = m_navigator->getPosition();
    m_position = "Position: " + QString::number(position.x) + ", " + QString::number(position.y);
    Q_EMIT positionChanged();
}

bool Beacons::state()
{
    return m_device->getScanState();
}

bool Beacons::rssiMeasState()
{
    return m_rssi_meas_state;
}

bool Beacons::checkMacAddress(QString mac_address)
{
    logMessage("Beacons::checkMacAddress:");
    logMessage(mac_address);
    if(m_beacons.contains(mac_address))
    {
        return true;
    }
    return false;
}

void Beacons::updateBeaconInfo(QString mac_address, qint16 rssi)
{
    if(m_mode_type == tracking)
    {
        updateDistance(mac_address, rssi);
    }
    else if(m_mode_type == rssi_measuring)
    {
        updateRssi(mac_address, rssi);
    }
}

void Beacons::updateDistance(QString mac_address, qint16 rssi)
{
    logMessage("Beacons::updateDistance");
    if(!checkMacAddress(mac_address))
    {
        return;
    }
    m_mutex.lock();
    m_beacons[mac_address].distance = m_calculator->calcDistance(rssi);
    m_mutex.unlock();
    logMessage("distance = " + QString::number(m_beacons[mac_address].distance));
}

void Beacons::updateRssi(QString mac_address, qint16 rssi)
{
    if(m_rssi_mac_address != "" && m_rssi_mac_address != mac_address)
    {
        return;
    }
    logMessage("Beacons::updateRssi");
    QTime current_time = QTime::currentTime();
    QString log = current_time.toString() + ".";
    int msec = current_time.msec();
    if(msec < 100)
    {
        log += "0";
        if(msec < 10)
        {
            log += "0";
        }
    }
    log += QString::number(msec) + " ";
    if(m_rssi_mac_address == "")
    {
        log += mac_address + " ";
    }
    log += QString::number(rssi) + "\r\n";
    m_logger->saveLog(m_rssi_log_file, log);
}

bool Beacons::validateMacAddress(QString mac_address)
{
    QRegExp rx("^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$");
    return (rx.indexIn(mac_address) != -1);
}

double Beacons::getDistance(QString mac_address)
{
    return m_beacons[mac_address].distance;
}

QList<DistanceToBeacon> Beacons::getDistances()
{
    return m_beacons.values();
}

Device *Beacons::getDevice()
{
    return m_device;
}

void Beacons::startTracking()
{
    logMessage("Beacons::startTracking");
    m_mode_type = tracking;
    startScan();
    startNavigate();
    setInfo("Tracking...");
}

void Beacons::stopTracking()
{
    logMessage("Beacons::stopTracking");
    stopScan();
    stopNavigate();
    setInfo(start_tracking_msg);
}

void Beacons::startMeasuring(QString mac_address)
{
    logMessage("Beacons::startMeasuring");
    logMessage("mac_address: " + mac_address);
    mac_address = mac_address.toUpper();
    //mac_address = "F6:C2:B1:B4:11:EC"; // TODO Remove after test's
    if(mac_address != "" && !validateMacAddress(mac_address))
    {
        setInfo("Invalid mac address provided");
        return;
    }
    m_mode_type = rssi_measuring;
    m_rssi_mac_address = mac_address;
    m_rssi_meas_state = true;
    Q_EMIT rssiMeasStateChanged();
    setInfo("Stop Measure");
    m_rssi_log_file = m_logger->createLogFile(m_rssi_mac_address);
    if(!m_logger->openLogFile(m_rssi_log_file))
    {
        qDebug() << "ERROR while opening file!\n";
        return;
    }
    startScan();
}

void Beacons::stopMeasuring()
{
    logMessage("Beacons::stopMeasuring");
    m_rssi_meas_state = false;
    Q_EMIT rssiMeasStateChanged();
    setInfo("Start Measure");
    m_logger->closeLogFile(m_rssi_log_file);
    stopScan();
}

void Beacons::editBeacon(QString mac_address, QString position)
{
    logMessage("Beacons::editBeacon");
    logMessage("mac_address: " + mac_address);
    logMessage("position: " + position);
    mac_address = mac_address.toUpper();
    if(!validateMacAddress(mac_address))
    {
        setInfo("Invalid mac address provided");
        return;
    }
    QStringList pos = position.split(',');
    double x = pos[0].toDouble();
    double y = pos[1].toDouble();
    if(m_beacons.contains(mac_address))
    {
        setInfo("Beacon position updated");
        m_beacons[mac_address].point = {x,y};
    }
    else
    {
        setInfo("New Beacon inserted");
        m_beacons.insert(mac_address, {{x,y}, 0});
    }
}

QString Beacons::getRssiMacAddress()
{
    return m_rssi_mac_address;
}

void Beacons::setRssiMacAddress(QString mac_address)
{
    m_rssi_mac_address = mac_address;
    Q_EMIT rssiMacAddressChanged();
}
