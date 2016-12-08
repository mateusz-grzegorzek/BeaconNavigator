#include "beacons.h"
#include "navigator.h"
#include "device.h"
#include "logger.h"
#include "calculator.h"
#include <QTextStream>
#include <QCoreApplication>

Beacons::Beacons():
   m_estimation_type(multilateration){
    loadBeacons();
}

void Beacons::initModules(){
    m_navigator = new Navigator(this);
    m_device = new Device(this);
}

void Beacons::loadBeacons(){
    Logger::logMessage("Beacons::loadBeacons");
    QFile beacons_file("/sdcard/Download/beacons.txt");
    if (!beacons_file.open(QIODevice::ReadOnly | QIODevice::Text)){
        Logger::logMessage("Can't open beacons file!\n");
        return;
    }
    QTextStream in(&beacons_file);
    while (!in.atEnd()){
        QString line = in.readLine();
        if(line.length() > 0){
            QStringList words = line.split(" ");
            QString mac_address = words[0];
            QStringList position = words[1].split(",");
            double x = position[0].toDouble();
            double y = position[1].toDouble();
            Logger::logMessage(mac_address + ": " + QString::number(x) + "," + QString::number(y));
            m_beacons.insert(mac_address, {{x,y}, 0});
        }
    }
}

void Beacons::startNavigation(){
    Logger::logMessage("Beacons::startNavigation");
    m_device->start();
}

void Beacons::deviceStarted(){
    m_rssi_log_file = Logger::createAndOpenLogFile("rssi");
    m_track_log_file = Logger::createAndOpenLogFile("pos");
    m_navigator->start();
    Q_EMIT navigatorStateChanged(true);
}

void Beacons::deviceFailed(){
    Logger::logMessage("Beacons::deviceFailed"); //TODO something
}

bool Beacons::isNavigating(){
    return m_navigator->isNavigating();
}

void Beacons::stopNavigation(){
    Logger::logMessage("Beacons::stopNavigation");
    Logger::closeLogFile(m_track_log_file);
    Logger::closeLogFile(m_rssi_log_file);
    m_device->turnOff();
    m_device->quit();
    m_device->wait();
    m_navigator->turnOff();
    m_navigator->quit();
    m_navigator->wait();
    Q_EMIT navigatorStateChanged(false);
}

void Beacons::updateBeaconInfo(QString mac_address, qint16 rssi){
    if(!checkMacAddress(mac_address)){
        return;
    }
    Logger::saveLogToFile(m_rssi_log_file, mac_address + " " + QString::number(rssi) );
    Calculator::calcMedian(mac_address, rssi);
    updateDistance(mac_address, rssi);
}

bool Beacons::checkMacAddress(const QString& mac_address){
    if(m_beacons.contains(mac_address)){
        Logger::logMessage("Beacons::checkMacAddress: " + mac_address);
        return true;
    }
    return false;
}

void Beacons::updatePosition(Point position){
    QString msg_position =  QString::number(position.x) + "," + QString::number(position.y);
    Logger::saveLogToFile(m_track_log_file, msg_position);
    Logger::logMessage("Position: " + msg_position);
    Q_EMIT positionChanged(position);
}

void Beacons::updateDistance(QString mac_address, qint16 rssi){
    m_beacons_mutex.lock();
    m_beacons[mac_address].distance = Calculator::calcDistance(rssi);
    m_beacons_mutex.unlock();
    Logger::logMessage(mac_address + "->distance = " + QString::number(m_beacons[mac_address].distance));
}

void Beacons::changeEstimation(){
    Logger::logMessage("Beacons::changeEstimation");
    if(m_estimation_type == multilateration){
        m_estimation_type = weighted_arith_mean;
    } else if(m_estimation_type == weighted_arith_mean){
        m_estimation_type = multilateration;
    }
}

estimation_type Beacons::getEstimationType(){
    return m_estimation_type;
}

QList<DistanceToBeacon> Beacons::getDistances(){
    return m_beacons.values();
}

Device* Beacons::getDevice(){
    return m_device;
}
