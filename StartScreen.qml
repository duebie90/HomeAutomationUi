import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id:root
    objectName: "StartScreen"
    anchors.fill: parent

    Connections {
        target: startScreen
        onSignalConnectFailed: {
            connectedString.text = reasonString
            connectedString.color = "red"
            console.log("StartScreen connect failed")

        }
        onSignalConnected: {
            console.log("StartScreen connected")
            connectedString.text = "Verbunden mit dem Server. Warten auf Bestätigung."
            connectedString.color = "green"
        }
        onSignalDisconnected: {
            console.log("StartScreen disconnected")
            connectedString.visible = false
        }
        onSignalHostError: {
            connectedString.text = reasonString
            connectedString.color = "red"
            console.log("Host resolution error")
        }
    }


    function connect() {
        startScreen.connectToServer(ipTextField.text, portTextField.text)
        connectedString.text = "Verbindungsaufbau..."
        connectedString.color ="white"
        connectedString.visible = true
    }

    Rectangle {
        id:background
        anchors.fill: parent
        color: "#262626"

    }
    Text {
        id:headline
        text:"Steuerzentrale Heimautomation"
        color:"#a9a9a9"
        font.pixelSize: 30
        font.bold: true
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Text {
        anchors.top: headline.bottom
        anchors.topMargin: 10
        anchors.left: background.left
        anchors.leftMargin: 5
        text: "Bitte IP und Port des Servers eingeben oder aus der Liste wählen."
        color: "#a9a9a9"
        font.pixelSize: 15
    }
    Rectangle {
        id:serverNetworkInfoContainer
        anchors.top: headline.bottom
        anchors.topMargin: 45
        anchors.left: background.left
        anchors.right: parent.right
        height: ipTextField.height + 10
        color:"#333333"
        border.color: "#003366"
        border.width: 2
        Text {
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            text:"Server IP Konfiguration"
            font.bold: true
            color: "#a9a9a9"
            font.pixelSize: 15
        }
        Text {
            id:ipLabel
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 300
            text: "IP:"
            color: "#a9a9a9"
            font.pixelSize: 20
        }
        TextField {
            id:ipTextField
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: ipLabel.right
            anchors.leftMargin: 5            
            text: recentHostName
            color: "black"
            selectByMouse: true
            focus:true
            Keys.onReturnPressed:{
                connect()
               }
        }
        Text {
            id:portLabel
            anchors.left: ipTextField.right
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            text: "Port: "
            color: "#a9a9a9"
            font.pixelSize: 20
        }
        TextField {
            id:portTextField
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: portLabel.right
            anchors.leftMargin: 10
            text: "3000"
            color: "black"
            selectByMouse: true
            validator: IntValidator{bottom:0; top:9999}
            Keys.onReturnPressed:{
                connect()
               }
        }
    }

    FrameButton {
        id:connectButton
        text:"verbinden"
        onSignalClicked:{   connect()}
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30

    }
    Text {
        id:connectedString
        anchors.centerIn: parent
        color: "white"
        font.pixelSize: 15
        visible: false
        width: 400
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        horizontalAlignment: Text.AlignHCenter
    }

}
