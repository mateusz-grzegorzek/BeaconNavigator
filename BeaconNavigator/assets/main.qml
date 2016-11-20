import QtQuick 2.0

Rectangle {
    id: back
    width: 300
    height: 600

    Header {
        id: header
        anchors.top: parent.top
        headerText: "Beacon Navigator"
    }

    Label{
        id: position
        anchors.top: header.bottom
        anchors.bottom: exit.top
        anchors.centerIn: parent
        text: beacons.position
        height: 200
    }

    Menu {
        id: track
        anchors.bottom: rssi_meas.top
        menuWidth: parent.width
        menuText: beacons.info
        onButtonClick: {
            if (!beacons.state) {
                beacons.startTracking();
            } else {
                beacons.stopTracking()
            }
        }
    }

    Menu {
        id: rssi_meas
        anchors.bottom: exit.top
        menuWidth: parent.width
        menuText: "Rssi Measurements"
        onButtonClick: {
            beacons.info = "Start Measure"
            pageLoader.source = "Rssi.qml"
        }
    }

    Menu {
        id: exit
        anchors.bottom: parent.bottom
        menuWidth: parent.width
        menuText: "Exit"
        onButtonClick: {
            beacons.exitApplication();
        }
    }

    Loader {
        id: pageLoader
        anchors.fill: parent
    }
}
