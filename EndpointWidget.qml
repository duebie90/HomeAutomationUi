import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0

Item {
    id:root
    visible: shown
    onVisibleChanged: console.log("on visible changed EPW")
    property int defaultX:200
    property variant endpoint: undefined
    property bool shown: false
    //if another slide was clicked before completed it is scheduled
    property bool hideScheduled: false
    property bool showScheduled: false
    onShownChanged: {
            if (shown) {
                show()
            }
            else {
                hide()
            }
    }

    function show() {
        if(hideAnimation.running) {
            //wait until hidden completed, then show again
            showScheduled = true
        } else {
            showAnimation.start()
        }
    }
    function hide() {
        if(showAnimation.running) {
            //wait until show compled, then hide again
            hideScheduled = true
        } else {
            hideAnimation.start()
        }
    }

    PropertyAnimation {
        id:showAnimation
        target: root
        property: "x"
        //from: defaultX - root.width
        to:defaultX
        duration:500
        easing.type: Easing.InOutQuad
        onStarted: root.visible = true
        onStopped: {
            if(hideScheduled) {
                hideScheduled = false
                hideAnimation.start()
            }
        }
    }
    PropertyAnimation {
        id:hideAnimation
        target: root
        property: "x"
        //from:defaultX
        to: root.width + defaultX
        duration:500
        easing.type: Easing.InOutQuad
        onStopped: {
            x = defaultX - root.width
            root.visible = false
            editSchedulesScreen.hide()
            if(showScheduled) {
                showScheduled = false
                showAnimation.start()
            }
        }
    }
    //onEndpointChanged: console.log("QmlEndpointWidget on endpoint changed")
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
        checked:(endpoint !== undefined ) ? switchedState : false
        onCheckedChanged: endpoint.requestStateChange(checked)        
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
        checked:(endpoint !== undefined ) ? autoMode : false
        onCheckedChanged: endpoint.changeAutoState(checked)
    }

    EditSchedulesScreen{
        id:editSchedulesScreen
        anchors.top: headerBar.bottom
        anchors.left: parent.left
        endpoint:root.endpoint
    }
}

