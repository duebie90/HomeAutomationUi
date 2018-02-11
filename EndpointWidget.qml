import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0

Item {
    id:root        
    property int defaultX:200
    property variant endpoint: undefined
    property bool shown: false
    //if another slide was clicked before completed it is scheduled
    property bool hideScheduled: false
    property bool showScheduled: false

    property bool switchedState: (endpoint !== undefined ) ? endpoint.switchedState : false
    onSwitchedStateChanged:  {
        manSwitch.checked = switchedState
    }
    property bool autoControlled: endpoint.autoMode
    onAutoControlledChanged: {
        autoSwitch.checked = autoControlled
    }



    function hideEditSchedulesScreen() {
        editSchedulesScreen.hide()
    }


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
        width:330
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top:headerBar.bottom
        anchors.topMargin: 50
        Column {
            spacing: 10
            FrameButton {
                width: 150
                text:"Zeitpläne"
                onSignalClicked: editSchedulesScreen.show()
            }
            FrameButton {
                width: 150
                text:"Zufallsmodus"
            }
        }
        Column {
            spacing: 10
            FrameButton {
                width: 150
                text:"Einmaliges Ereignis"
            }
            FrameButton {
                width: 150
                text:"Werkseinstellungen"
            }
        }
    }
    Text{
        id:manSwitchCaption
        text:"Manuell"
        anchors.top: buttonArray.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -10 - manSwitch.width
        color:"#a9a9a9"
        font.pixelSize: 20
        font.bold: true
    }
    CustomSwitch {
        id:manSwitch
        anchors.top: manSwitchCaption.bottom
        anchors.topMargin: 2
        anchors.left: manSwitchCaption.left
        //checked:endpoint.switchedState //(endpoint !== undefined ) ? endpoint.switchedState : false
        checked: root.switchedState
        onCheckedChanged: endpoint.requestStateChange(checked)
        enabled: endpoint !== undefined && endpoint !== null
    }
    Text{
        id:autoSwitchCaption
        text:"Automatik"
        anchors.top: buttonArray.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 10 + autoSwitch.width
        color:"#a9a9a9"
        font.pixelSize: 20
        font.bold: true
    }
    CustomSwitch {
        id:autoSwitch
        anchors.top: autoSwitchCaption.bottom
        anchors.topMargin: 2
        anchors.left: autoSwitchCaption.left
        checked:(endpoint !== undefined ) ? endpoint.autoMode : false
        onCheckedChanged: endpoint.requestAutoMode(checked)
    }

    EditSchedulesScreen{
        id:editSchedulesScreen
        anchors.top: headerBar.bottom
        anchors.left: parent.left
        endpoint:root.endpoint
    }
}

