import QtQuick 2.0

Rectangle {
    id: back
    width: 300
    height: 600

    Header {
        id: header
        anchors.top: parent.top
        headerText: "Rssi measurements"
    }

    InputBox {
        id: mac_address
        labelText: "Mac address:"
    }

    Menu {
        id: start
        anchors.bottom: return_to_main.top
        menuWidth: parent.width
        menuText: beacons.info
        onButtonClick: {
            if (!beacons.rssiMeasState) {
                beacons.startMeasuring(mac_address.inputText);
            } else {
                beacons.stopMeasuring()
            }
        }
    }

    Menu {
        id: return_to_main
        anchors.bottom: parent.bottom
        menuWidth: parent.width
        menuText: "Return"
        onButtonClick: {
            beacons.info = "Start Tracking";
            pageLoader.source = "main.qml"
        }
    }

    Loader {
        id: pageLoader
        anchors.fill: parent
    }
}
