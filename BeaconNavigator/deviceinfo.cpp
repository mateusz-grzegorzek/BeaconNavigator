#include <qbluetoothuuid.h>
#include "deviceinfo.h"

DeviceInfo::DeviceInfo()
{
}

DeviceInfo::DeviceInfo(const QBluetoothDeviceInfo &d)
{
    device = d;
}

QString DeviceInfo::getAddress() const
{
    return device.address().toString();
}

QString DeviceInfo::getName() const
{
    return device.name();
}

QString DeviceInfo::getStringRssi() const
{
    return QString::number(device.rssi());
}

qint16 DeviceInfo::getRssi() const
{
    return device.rssi();
}

QBluetoothDeviceInfo DeviceInfo::getDevice()
{
    return device;
}

