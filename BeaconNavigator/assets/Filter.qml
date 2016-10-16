import QtQuick 2.0

Rectangle {
    width: 300
    height: 600

    Header {
        id: header
        anchors.top: parent.top
        headerText: "Filter Beacons"
    }

    ListView {
        id: theListView
        height: parent.height
        width: parent.width
        clip: true

        anchors.top: header.bottom
        anchors.bottom: filterByRsii.top
        model: device.devicesList

        delegate: Rectangle {
            id: box
            height: 200
            width: parent.width
            color: "lightsteelblue"
            border.width: 2
            border.color: "black"
            radius: 5

            Label {
                id: deviceName
                textContent: modelData.deviceName
                anchors.top: box.top
                anchors.topMargin: 5
            }

            Label {
                id: deviceAddress
                textContent: modelData.deviceAddress
                font.pointSize: deviceName.font.pointSize*0.7
                anchors.centerIn: box
            }

            Label {
                id: deviceRssi
                textContent: modelData.deviceRssi
                font.pointSize: deviceName.font.pointSize*0.7
                color: "red"
                anchors.bottom: box.bottom
                anchors.bottomMargin: 5
            }
        }
    }

    Menu {
        id: filterByRsii
        anchors.bottom: back.top
        anchors.left: parent.left
        menuWidth: (parent.width/2)
        menuText: "Filter by RSSI"
        onButtonClick: {
            device.filterBeaconsByRssi();
        }
    }

    Menu {
        id: filterByMac
        anchors.bottom: back.top
        anchors.right: parent.right
        menuWidth: (parent.width/2)
        menuText: "Filter by MAC"
        onButtonClick: {
            device.filterBeaconsByMacAddresses();
        }
    }

    Menu {
        id: back
        anchors.bottom: parent.bottom
        menuWidth: parent.width
        menuText: "Back"
        onButtonClick: {
            pageLoader.source = "main.qml"
        }
    }
}
