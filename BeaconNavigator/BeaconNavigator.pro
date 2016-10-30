TARGET = BeaconNavigator
INCLUDEPATH += . \
                googletest \
                googletest/include

QT += quick bluetooth

# Input
SOURCES += main.cpp \
    device.cpp \
    deviceinfo.cpp \
    beacons.cpp \
    calculator.cpp \
    navigator.cpp \
    tests.cpp

OTHER_FILES += assets/*.qml

HEADERS += \
    device.h \
    deviceinfo.h \
    beacons.h \
    calculator.h \
    navigator.h \
    point.h \
    distancetobeacon.h

RESOURCES += \
    resources.qrc

CONFIG(Desktop): LIBS += -lgtest -L/usr/include/gtest
CONFIG(Desktop): DEFINES += G_TEST

target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/BeaconNavigator
INSTALLS += target

DISTFILES +=
