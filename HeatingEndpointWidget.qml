import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0

Item {
    id:root
    width:600
    property variant endpoint: undefined
    Rectangle {
        id:backgroundColor
        anchors.fill: parent
        color: "#262626"
        border.color: "#a9a9a9"
        border.width: 1
    }
    Rectangle {
        id:headerBar
        property bool connected: (endpoint !== undefined ) ? endpoint.connected : false
        color: connected ? "green" : "#a9a9a9"
        Behavior on color {ColorAnimation {duration:200}}
        border.color: "white"
        border.width: 1
        anchors.top: parent.top
        width: parent.width
        height: 100
        Text {
            id:headlineAlias
            anchors.top: parent.top
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            text: (endpoint !== undefined ) ? endpoint.name : ""
            color:"#333333"
            font.pixelSize: 35
            font.bold: true
        }
        Text {
            id:subheadingType
            anchors.top: headlineAlias.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            text: (endpoint !== undefined ) ? endpoint.type: ""
            color:"#333333"
            font.pixelSize: 20
        }
        Text {
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            text:"löschen"
            font.bold: true
            font.pixelSize: 15
            visible: (opacity > 0) ? true : false
            opacity: headerBar.connected ? 0 : 1
            Behavior on opacity { PropertyAnimation{duration:200}}
            color: deleteScheduleMouseArea.pressed ? "brightRed": "darkRed"
            MouseArea {
                id:deleteScheduleMouseArea
                anchors.fill: parent
                onClicked: endpoint.remove()
            }
        }
    }


    Row {
        id:buttonArray
        spacing: 10
        width:parent.width -20
        anchors.top:headerBar.bottom
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        Column {
            spacing: 10
            Rectangle{
                    width: 290
                    height:300
                    id:groupBoxBackgroundLeft
                    color:"transparent"
                Text{
                    id:headlineHeaterCircuit
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    font.pixelSize: 20
                    color:"#a9a9a9"
                    text: "Heizungsvorlauf"
                }
                Rectangle{
                    id:heaterCurrentInfluxTempBackground
                    anchors.top:headlineHeaterCircuit.bottom
                    anchors.topMargin:20
                    height:50
                    width:parent.width
                    color:"#333333"
                    Text{
                        id:heaterCurrentInfluxTempLabel
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 15
                        text:"Vorlauftemperatur"
                        color:"white"

                    }
                    TextField{
                        id:heaterCurrentInfluxTemp
                        anchors.right: parent.right
                        anchors.rightMargin:20
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 15
                        text: "45°C"
                        width: 50
                        readOnly: true
                    }
                }
                Rectangle{
                    id:heaterTargetInfluxTempBackground
                    anchors.top:heaterCurrentInfluxTempBackground.bottom
                    anchors.topMargin:20
                    height:50
                    width:parent.width
                    color:"#333333"
                    Text{
                        id:heaterTargetInfluxTempLabel
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 15
                        text:"Sollwert Vorlauftemperatur: "
                        color:"white"

                    }
                    Text{
                        id:heaterTargetInfluxTemp
                        anchors.left: heaterTargetInfluxTempLabel.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 15
                        text:"45°C"
                        color:"green"
                    }
                }
                FrameButton {
                    anchors.left: parent.left
                    anchors.top: heaterTargetInfluxTempBackground.bottom
                    anchors.topMargin:20
                    width: 150
                    text:"Solltemperatur ändern"
                    onSignalClicked: editSchedulesScreen.show()
                }
            }
        }
        Column {
            spacing: 10
            Rectangle{
                    id:groupBoxBackgroundRight
                    width: 290
                    height:300
                    color:"transparent"
                Text{
                    id:headlineBoiler
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    font.pixelSize: 20
                    color:"#a9a9a9"
                    text: "Kessel"
                }
                Rectangle{
                    id:boilerTempBackground
                    anchors.top:headlineBoiler.bottom
                    anchors.topMargin:20
                    height:50
                    width:parent.width
                    color:"#333333"
                    Text{
                        id:boilerCurrentTempLabel
                        anchors.left: parent.left
                        font.pixelSize: 15
                        text:"aktuelle Kesseltemperatur"
                        color:"white"

                    }
                    TextField{
                        id:boilerCurrentTemp
                        anchors.right: parent.right
                        anchors.rightMargin:20
                        font.pixelSize: 15
                        text: "75°C"
                        width: 50
                        readOnly: true
                    }
                }
                Rectangle{
                    id:boilerWarnThresholdBackground
                    anchors.top:boilerTempBackground.bottom
                    anchors.topMargin:20
                    height:50
                    width:parent.width
                    color: "#333333"
                    Text{
                        id:boilerWarnThresholdLabel
                        anchors.left: parent.left
                        font.pixelSize: 15
                        text:"Warnschwelle"
                        color:"white"
                    }
                    TextField{
                        id:boilerWarnThreshold
                        anchors.right: parent.right
                        anchors.rightMargin:20
                        font.pixelSize: 15
                        color:"darkred"
                        text: "90°C"
                        width: 50
                        readOnly: true
                    }
                }
            }

        }
    }
}

