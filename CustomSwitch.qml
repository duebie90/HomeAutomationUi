import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0

Switch {
    id: control
    down: handleMouseArea.pressed    
    onCheckedChanged: {
        console.log("onCheckChanged")
        if(checked) {
            handle.x = handleMouseArea.drag.maximumX
        } else {
            handle.x = handleMouseArea.drag.minimumX
        }
    }

    indicator: Rectangle {
        id:indicatorRect
        implicitWidth: 120
        implicitHeight: 30
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 25
        color: control.checked ? "#336699" : "#cccccc"

        Rectangle {
            id:handle
            x: control.checked ? handleMouseArea.drag.maximumX : 0
            anchors.verticalCenter: parent.verticalCenter
            Behavior on x {NumberAnimation {duration:50; easing.type: Easing.InOutQuad} }
            width: 50
            height: 50
            radius: 25
            color: control.down ? "#8c8c8c" : "#ffffff"
            border.color: "#333333"
            MouseArea {
                id:handleMouseArea
                anchors.fill: parent
                drag.target: parent
                drag.axis: Drag.XAxis
                drag.maximumX: indicatorRect.width - width
                drag.minimumX: 0
                property int thresholdX: checked ? 3*indicatorRect.width/4 : 1*indicatorRect.width/4
                onReleased:  {
                    if (handle.x > (thresholdX - width/2)) {
                        handle.x = drag.maximumX
                        control.checked = true
                    } else {
                        handle.x = drag.minimumX
                        control.checked = false
                    }
                }
                onClicked:{ if(checked) {
                                checked = false
                            } else {
                                checked = true
                            }
                        }
            }

        }

    }

}
