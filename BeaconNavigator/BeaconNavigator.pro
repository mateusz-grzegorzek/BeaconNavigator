TARGET = BeaconNavigator
INCLUDEPATH += .

QT += quick bluetooth
QT += network

# Input
SOURCES += main.cpp \
    device.cpp \
    deviceinfo.cpp \
    beacons.cpp \
    calculator.cpp \
    navigator.cpp \
    tcpserver.cpp \
    logger.cpp \
    loggerinterface.cpp

OTHER_FILES += assets/*.qml

HEADERS += \
    device.h \
    deviceinfo.h \
    beacons.h \
    calculator.h \
    navigator.h \
    point.h \
    distancetobeacon.h \
    tcpserver.h \
    logger.h \
    loggerinterface.h

RESOURCES += \
    resources.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/BeaconNavigator
INSTALLS += target

DISTFILES +=
