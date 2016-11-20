import QtQuick 2.0

Rectangle {
    id: back
    width: 300
    height: 600

    Header {
        id: header
        anchors.top: parent.top
        headerText: "Beacon Editor"
    }

    InputBox {
        id: mac_address
        anchors.top: header.bottom
        labelText: "Mac address:"
    }

    InputBox {
        id: position
        anchors.top: mac_address.bottom
        labelText: "Position:"
    }

    Menu {
        id: edit
        anchors.bottom: return_to_main.top
        menuWidth: parent.width
        menuText: beacons.info
        onButtonClick: {
            beacons.editBeacon(mac_address.inputText, position.inputText);
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
