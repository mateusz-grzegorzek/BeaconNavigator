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

void Device::run()
{
    qRegisterMetaType<QBluetoothDeviceDiscoveryAgent::Error>("QBluetoothDeviceDiscoveryAgent::Error");
    m_deviceScanState = true;
    while(m_deviceScanState)
    {
        startDeviceDiscovery();
        QThread::sleep(3);
        stopDeviceDiscovery();
    }
}

Device::Device(Beacons* beacons):
    m_deviceScanState(false), m_beacons(beacons)
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    connect(discoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
            this, SLOT(addDevice(const QBluetoothDeviceInfo&)));
    connect(discoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
            this, SLOT(deviceScanError(QBluetoothDeviceDiscoveryAgent::Error)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(deviceScanFinished()));
}

Device::~Device()
{
    delete discoveryAgent;
}

void Device::startDeviceDiscovery()
{
    qDebug() << "Device::startDeviceDiscovery";
    discoveryAgent->start();
}

void Device::stopDeviceDiscovery()
{
    qDebug() << "Device::stopDeviceDiscovery";
    discoveryAgent->stop();
}

void Device::turnOff()
{
    qDebug() << "Device::turnOff";
    m_deviceScanState = false;
}

bool Device::getScanState()
{
    return m_deviceScanState;
}

void Device::addDevice(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {
        qDebug() << "addDevice: ";
        qDebug() << "name = " << info.name();
        qDebug() << "address = " << info.address();
        qDebug() << "rssi = " << info.rssi();
        if(deviceType(info) == my_beacon)
        {
            qDebug() << "Found my beacon!!!";
            m_beacons->updateDistance(info.address().toString(), info.rssi());
        }
    }
}

void Device::deviceScanFinished()
{
    qDebug() << "deviceScanFinished";
    m_deviceScanState = false;
}

void Device::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        m_beacons->setInfo("The Bluetooth adaptor is powered off, power it on before doing discovery.");
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        m_beacons->setInfo("Writing or reading from the device resulted in an error.");
    else
        m_beacons->setInfo("An unknown error has occurred.");
    m_deviceScanState = false;
}

device_type Device::deviceType(const QBluetoothDeviceInfo &info)
{
    if(m_beacons->checkMacAddress(info.address().toString()))
    {
        return my_beacon;
    }
    return other_device;
}
