import QtQuick 2.0
import QtGraphicalEffects 1.0
Item {
    id:root
    signal signalClicked
    property bool pressed: buttonMouseArea.pressed
    property alias text: buttonCaption.text
    property bool selected: false
    width:200
    height:30
    visible: true
    Keys.onPressed:{
        if (event.key === Qt.Key_Space) {
            signalClicked()
            root.pressed = true
        }
    }
    Keys.onReleased: {
        root.pressed = false
    }

    RectangularGlow {
        id: backgroundGlow
        anchors.fill:buttonForeground
        glowRadius: (buttonMouseArea.pressed || root.pressed) ? 10 : 5
        spread: (buttonMouseArea.pressed || root.pressed) ? 0.2 : 0.0
        color:root.selected || pressed ?"#99ffcc" :"white"
        Behavior on color { PropertyAnimation {duration:50 } }
        Behavior on spread { NumberAnimation {duration:50 } }
    }
    Rectangle {
        id:buttonForeground
        anchors.fill:parent
        border.color: "#888888"
        border.width: 2
        radius: 10
        color: buttonMouseArea.pressed || root.pressed ? "#333333" : "black"
        MouseArea {
            id:buttonMouseArea
            anchors.fill: parent
            onClicked:{
                signalClicked()
            }
            onPressed: root.focus = true

        }
        Text {
            id:buttonCaption
            color:"white"
            font.pixelSize: 15
            anchors.centerIn: parent
        }
    }
}

