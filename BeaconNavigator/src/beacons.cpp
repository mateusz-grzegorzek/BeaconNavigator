#include "beacons.h"
#include <QThread>
#include <QCoreApplication>
#include <QDebug>
#include "tcpserver.h"
#include <QTime>

Beacons::Beacons():
   m_estimation_type(multilateration), m_mode_type(tracking)
{
    setInfo(start_tracking_msg);
    // Beacon's
    //m_beacons.insert("F6:C2:B1:B4:11:EC", {{0,0}, 0});
    //m_beacons.insert("E3:6B:7D:9B:A2:82", {{3,2}, 0});

    // Beacon;s Pro -one channel adv
    m_beacons.insert("C3:FD:C2:D1:73:6E", {{0,0}, 1}); // czarny kabelek
    m_beacons.insert("C4:2F:31:C4:3C:22", {{3,0}, 1}); // biały krótki kabelek
    m_beacons.insert("C2:7A:4B:B8:C3:33", {{1,2}, 1}); // biały kabelek
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
#if(G_TEST == 0)
    m_track_log_file = m_logger->createLogFile("position");
    m_logger->openLogFile(m_track_log_file);
#endif
}

void Beacons::stopNavigate()
{
    logMessage("Beacons::stopNavigate");
#if(G_TEST == 0)
    m_logger->closeLogFile(m_track_log_file);
#endif
    m_navigator->turnOff();
    m_navigator->quit();
    m_navigator->wait();
}

void Beacons::logPosition(Point point)
{
    QString log_position = m_logger->getTimeStamp();
    log_position += ": " + QString::number(point.x) + "," + QString::number(point.y);
    m_logger->saveLog(m_track_log_file, log_position);
}

void Beacons::exitApplication()
{
    QCoreApplication::quit();
}

QString Beacons::getInfo()
{
    return m_info;
}

QString Beacons::getEstimationInfo()
{
    if(m_estimation_type == multilateration)
    {
        return "Multilateration";
    }
    else if(m_estimation_type == weightedArithMean)
    {
        return "Weighted Arith Mean";
    }
    return "Unknown";
}

estimation_type Beacons::getEstimationType()
{
    return m_estimation_type;
}

void Beacons::setInfo(QString info)
{
    m_info = info;
    Q_EMIT infoChanged();
}

void Beacons::setEstimationInfo(estimation_type type)
{
    m_estimation_type = type;
    Q_EMIT estimationInfoChanged();
}

QString Beacons::getPosition()
{
    return m_position;
}

void Beacons::updatePosition()
{
    Point position = m_navigator->getPosition();
    m_position = "Position: " + QString::number(position.x) + ", " + QString::number(position.y);
    logPosition(position);
    logMessage(m_position);
    Q_EMIT positionChanged();
}

bool Beacons::trackingState()
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
        logMessage("My beacon founded!");
        return true;
    }
    logMessage("Other device founded.");
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
    QString log = m_logger->getTimeStamp() + " ";
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
    QThread::sleep(1);
    if(trackingState())
    {
        startNavigate();
        setInfo("Tracking...");
    }
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
        logMessage("Invalid mac address provided");
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
        logMessage("Invalid mac address provided");
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

void Beacons::changeEstimation()
{
    logMessage("Beacons::changeEstimation");
    if(m_estimation_type == multilateration)
    {
        m_estimation_type = weightedArithMean;
    }
    else if(m_estimation_type == weightedArithMean)
    {
        m_estimation_type = multilateration;
    }
    logMessage(getEstimationInfo());
    Q_EMIT estimationInfoChanged();
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
