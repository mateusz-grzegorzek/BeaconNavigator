TARGET = BeaconNavigator
INCLUDEPATH += . \
                googletest \
                googletest/include \
                $$PWD/hdr

QT += quick bluetooth
QT += network
QT += widgets

# Input
SOURCES += src/*.cpp

OTHER_FILES += assets/*.qml

HEADERS += hdr/*.h \
    hdr/point.h \
    hdr/wall.h

RESOURCES += \
    resources.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/BeaconNavigator
INSTALLS += target

DISTFILES += \
    beacons.txt \
    flat.txt
