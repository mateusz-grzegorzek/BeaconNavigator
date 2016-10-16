TARGET = BeaconNavigator
INCLUDEPATH += .

QT += quick bluetooth

# Input
SOURCES += main.cpp \
    device.cpp \
    deviceinfo.cpp \
    beaconfilter.cpp

OTHER_FILES += assets/*.qml

HEADERS += \
    device.h \
    deviceinfo.h \
    beaconfilter.h \
    beaconsmacaddresses.h

RESOURCES += \
    resources.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/BeaconNavigator
INSTALLS += target

DISTFILES += \
    assets/Filter.qml
