import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0

Item {
    id:root
    visible:true
    anchors.fill:parent
    Rectangle {
        id:backgroundColor
        anchors.fill: parent
        color: "#808080"
    }
    Rectangle {
        id:headerBar
        color: endpoint.connected ? "green" : "darkGrey"
        anchors.top: parent
        width: parent.width
        height: 200
        Text {
            id:headlineAlias
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            text: endpoint.alias
            font.pixelSize: 35
            font.bold: true
        }
        Text {
            id:subheadingType
            anchors.top: headlineAlias.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            text: endpoint.type
            font.pixelSize: 20
        }
    }
    BusyIndicator {
        id: busyIndicator1
        anchors.top: headerBar.bottom
        anchors.topMargin: 20
    }

    Switch {
        id: switch1
        anchors.top: busyIndicator1.bottom
        anchors.topMargin: 20
        checked:endpoint.switchedOn
        onCheckedChanged: endpoint.requestStateChange(checked)
        text: qsTr("Eingeschaltet")
    }

    Calendar {
        id: calendar1
        anchors.top: headerBar.bottom
        anchors.topMargin: 20
        anchors.right: parent.right
        onSelectedDateChanged: console.log("You selected: " + selectedDate)

    }

}

