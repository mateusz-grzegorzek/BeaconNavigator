#include "beacons.h"
#include <QThread>
#include <QCoreApplication>
#include <QDebug>
#include "tcpserver.h"
#include <QTime>
#include <QtAlgorithms>

Beacons::Beacons():
   m_estimation_type(multilateration),
   m_mode_type(median_tracking),
   m_filter_strength(0.5),
   m_filter_type(third_order)
{
    setInfo(start_tracking_msg);
    // Beacon's
    //m_beacons.insert("F6:C2:B1:B4:11:EC", {{0,0}, 0});
    //m_beacons.insert("E3:6B:7D:9B:A2:82", {{3,2}, 0});

    // Beacon's Pro -one channel adv
    //m_beacons.insert("C3:FD:C2:D1:73:6E", {{0,0}, 0}); // czarny kabelek
    //m_beacons.insert("C4:2F:31:C4:3C:22", {{3,0}, 0}); // biały krótki kabelek
    //m_beacons.insert("C2:7A:4B:B8:C3:33", {{1,2}, 0}); // biały kabelek
    loadBeacons();
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

void Beacons::loadBeacons()
{
    logMessage("Beacons::loadBeacons");
    QFile file("/sdcard/Download/beacons.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        logMessage("Can't open beacons file!\n");
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList words = line.split(" ");
        QString mac_address = words[0];
        QStringList position = words[1].split(",");
        double x = position[0].toDouble();
        double y = position[1].toDouble();
        logMessage(mac_address + ": " + QString::number(x) + "," + QString::number(y) + "\n" );
        m_beacons.insert(mac_address, {{x,y}, 0});
    }
}

void Beacons::printRssi(qint16 rssi)
{
    m_rssi = "Rssi = " + QString::number(rssi);
    Q_EMIT rssiChanged();
}

void Beacons::createAndOpenRssiLogFile()
{
    m_rssi_log_file = m_logger->createLogFile(m_rssi_mac_address);
    if(!m_logger->openLogFile(m_rssi_log_file))
    {
        logMessage("ERROR while opening file!\n");
        return;
    }
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
    if(m_mode_type == rssi_measuring)
    {
        return;
    }
    logMessage("Beacons::startNavigate");
    setInfo("Tracking...");
    m_apply_filter = false;
    m_navigator->start();
#if(G_TEST == 0)
    createAndOpenRssiLogFile();
    m_track_log_file = m_logger->createLogFile("position");
    m_logger->openLogFile(m_track_log_file);
#endif
}

void Beacons::stopNavigate()
{
    logMessage("Beacons::stopNavigate");
#if(G_TEST == 0)
    m_logger->closeLogFile(m_track_log_file);
    m_logger->closeLogFile(m_rssi_log_file);
#endif
    m_navigator->turnOff();
    m_navigator->quit();
    m_navigator->wait();
}

void Beacons::logPosition(Point point)
{
    QString log_position = m_logger->getTimeStamp();
    log_position += ": " + QString::number(point.x) + "," + QString::number(point.y) + "\n";
    m_logger->saveLog(m_track_log_file, log_position);
}

Point Beacons::applyFilter(Point point)
{
    logMessage("Beacons::applyFilter");
    logMessage("filter_type = " + QString::number(m_filter_type));
    if(m_filter_type == second_order)
    {
        if(m_apply_filter)
        {
            point.x = (point.x + m_last_point.x)*m_filter_strength;
            point.y = (point.y + m_last_point.y)*m_filter_strength;
        }
        else
        {
            m_apply_filter = true;
        }
        m_last_point = point;
    }
    else if(m_filter_type == third_order)
    {
        if(m_last_points.length() < 3)
        {
            m_last_points.append(point);
        }
        if(m_last_points.length() > 2)
        {
            point.x = 0;
            point.y = 0;
            for(Point p: m_last_points)
            {
                point.x += p.x;
                point.y += p.y;
            }
            point.x /= 3;
            point.y /= 3;
            m_last_points.erase(m_last_points.begin());
        }

    }
    return point;
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

QString Beacons::getRssi()
{
    return m_rssi;
}

void Beacons::updatePosition()
{
    Point position = m_navigator->getPosition();
    logMessage("Before Filter: " + QString::number(position.x) + ", " + QString::number(position.y));
    position = applyFilter(position);
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
    if(!checkMacAddress(mac_address))
    {
        return;
    }
    if(m_mode_type == tracking)
    {
        updateDistance(mac_address, rssi);
    }
    else if(m_mode_type == median_tracking)
    {
        medianTracking(mac_address, rssi);
        updateDistance(mac_address, rssi);
        updateRssi(mac_address, rssi);
    }
    else if(m_mode_type == rssi_measuring)
    {
        medianTracking(mac_address, rssi);
        updateRssi(mac_address, rssi);
        printRssi(rssi);
    }
}

void Beacons::updateDistance(QString mac_address, qint16 rssi)
{
    logMessage("Beacons::updateDistance");
    m_mutex.lock();
    m_beacons[mac_address].distance = m_calculator->calcDistance(rssi);
    m_mutex.unlock();
    logMessage("distance = " + QString::number(m_beacons[mac_address].distance));
}

void Beacons::medianTracking(QString mac_address, qint16& rssi)
{
    logMessage("Beacons::medianTracking");
    if(m_median_cache[mac_address].length() < 3)
    {
        m_median_cache[mac_address].append(rssi);
    }
    if(m_median_cache[mac_address].length() > 2)
    {
        QList<qint16> rssi_cache = m_median_cache[mac_address];
        qSort(rssi_cache);
        rssi = rssi_cache[1];
        logMessage("median_rssi = " + QString::number(rssi));
        m_median_cache[mac_address].erase(m_median_cache[mac_address].begin());
    }
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
    m_mode_type = median_tracking;
    startScan();
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
    mac_address = "C2:7A:4B:B8:C3:33";  // TODO Remove after test's
    logMessage("mac_address: " + mac_address);
    mac_address = mac_address.toUpper();
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
    createAndOpenRssiLogFile();
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
