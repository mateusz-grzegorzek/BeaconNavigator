/***************************************************************************
**
** Copyright (C) 2013 BlackBerry Limited. All rights reserved.
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "device.h"
#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QDebug>
#include <QList>
#include <QTimer>
#include <QCoreApplication>

Device::Device():
    m_deviceScanState(false)
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    connect(discoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
            this, SLOT(addDevice(const QBluetoothDeviceInfo&)));
    connect(discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
            this, SLOT(deviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(deviceScanFinished()));

    setUpdate("Search");
}

Device::~Device()
{
    delete m_beacon_filter;
    delete discoveryAgent;
    qDeleteAll(devices);
    devices.clear();
}

void Device::setBeaconFilter(BeaconFilter *beacon_filter)
{
    m_beacon_filter = beacon_filter;
}

void Device::startDeviceDiscovery()
{
    qDebug() << "startDeviceDiscovery!";
    qDeleteAll(devices);
    devices.clear();
    Q_EMIT devicesUpdated();

    setUpdate("Scanning for devices ...");
    discoveryAgent->start();

    if (discoveryAgent->isActive()) {
        m_deviceScanState = true;
        Q_EMIT stateChanged();
    }
}

void Device::stopDeviceDiscovery()
{
    qDebug() << "stopDeviceDiscovery!";
    discoveryAgent->stop();
    Q_EMIT devicesUpdated();
    m_deviceScanState = false;
    Q_EMIT stateChanged();
    setUpdate("Search");
}

void Device::filterBeaconsByMacAddresses()
{
    devices = m_beacon_filter->filterBeaconsByMacAddresses(devices);
    Q_EMIT devicesUpdated();
}

void Device::filterBeaconsByRssi()
{
    devices = m_beacon_filter->filterBeaconsByRssi(devices);
    Q_EMIT devicesUpdated();
}

void Device::exitApplication()
{
    QCoreApplication::quit();
}

void Device::addDevice(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        qDebug() << "addDevice: ";
        qDebug() << "name = " << info.name();
        qDebug() << "address = " << info.address();
        qDebug() << "rssi = " << info.rssi();
        DeviceInfo *d = new DeviceInfo(info);
        devices.append(d);
    }
}

void Device::deviceScanFinished()
{
    qDebug() << "deviceScanFinished!";
    Q_EMIT devicesUpdated();
    m_deviceScanState = false;
    Q_EMIT stateChanged();
    setUpdate("Search");
}

QVariant Device::getDevices()
{
    return QVariant::fromValue(devices);
}

QList<QObject *>* Device::getBeacons()
{
    return &devices;
}

QString Device::getUpdate()
{
    return m_message;
}

void Device::setUpdate(QString message)
{
    m_message = message;
    Q_EMIT updateChanged();
}

void Device::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        setUpdate("The Bluetooth adaptor is powered off, power it on before doing discovery.");
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        setUpdate("Writing or reading from the device resulted in an error.");
    else
        setUpdate("An unknown error has occurred.");

    m_deviceScanState = false;
    Q_EMIT devicesUpdated();
    Q_EMIT stateChanged();
}

bool Device::state()
{
    return m_deviceScanState;
}
