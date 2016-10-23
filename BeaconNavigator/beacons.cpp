#include "beacons.h"
#include <QThread>
#include <QCoreApplication>
#include <QDebug>

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

Beacons::~Beacons()
{

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
    qDebug() << "Beacons::startScan";
    m_device->start();
}

void Beacons::stopScan()
{
    qDebug() << "Beacons::stopScan";
    m_device->turnOff();
    m_device->quit();
    m_device->wait();
}

void Beacons::startNavigate()
{
    qDebug() << "Beacons::startNavigate";
    m_navigator->start();
}

void Beacons::stopNavigate()
{
    qDebug() << "Beacons::stopNavigate";
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

bool Beacons::checkMacAddress(QString mac_address)
{
    qDebug() << "Beacons::checkMacAddress";
    qDebug() << mac_address;
    if(m_beacons.contains(mac_address))
    {
        qDebug() << "Contains!!!";
        return true;
    }
    return false;
}

void Beacons::updateDistance(QString mac_address, qint16 rssi)
{
    qDebug() << "Beacons::updateDistance";
    m_mutex.lock();
    m_beacons[mac_address].distance = m_calculator->calcDistance(rssi);
    m_mutex.unlock();
    qDebug() << "distance = " << m_beacons[mac_address].distance;
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
    qDebug() << "Beacons::startTracking";
    startScan();
    startNavigate();
    setInfo("Tracking...");
}

void Beacons::stopTracking()
{
    qDebug() << "Beacons::stopTracking";
    stopScan();
    stopNavigate();
    setInfo(start_tracking_msg);
}
