TARGET = BeaconNavigator
INCLUDEPATH += . \
                googletest \
                googletest/include \
                $$PWD/hdr

QT += quick bluetooth
QT += network

# Input
SOURCES += src/*.cpp

OTHER_FILES += assets/*.qml

HEADERS += hdr/*.h

RESOURCES += \
    resources.qrc

CONFIG(Desktop): LIBS += -lgtest -L/usr/include/gtest
CONFIG(Desktop): DEFINES += G_TEST

target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/BeaconNavigator
INSTALLS += target
