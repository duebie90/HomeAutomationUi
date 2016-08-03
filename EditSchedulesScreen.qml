import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4
import QtQuick.Extras 1.4
Item {
    id:root
    visible: false
    onVisibleChanged: console.log("EditSchedules visible changed")
    opacity: 0
    height:500
    width:600
    property variant endpoint: undefined

    function show() {
        showAnimation.start()
        //opacity = 1
        //visible = true#
        console.log("EditSchedulesScreen show()")
    }
    function hide() {
        hideAnimation.start()
    }

    PropertyAnimation on opacity{
        id:showAnimation
        from:0;
        to: 1;
        duration:200
        running:false
        onStarted:{console.log("show Animation started")
            root.visible = true
        }
    }
    PropertyAnimation on opacity{
        id:hideAnimation
        from:1;
        to: 0;
        duration:200
        running:false
        onStopped:{
            console.log("hide Animation finished")
            root.visible = false
        }
    }

    Rectangle {
        id:backgroundColor
        anchors.fill: parent
        color: "#262626"
        border.color: "white"
        border.width: 1
        MouseArea{
            anchors.fill:parent
            //mouse area to prevent click onto underlying view
        }

    }
    Text{
        id:headline
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        text:"Zeitpläne bearbeiten, hinzufüge oder entfernen"
        color:"#a9a9a9"
        font.pixelSize: 20
        font.bold: true
    }
//    ComboBox {
//        id:schedulesCombo
//        anchors.top: headline.bottom
//        anchors.topMargin: 10
//        anchors.left: parent.left
//        anchors.leftMargin: 20
//        model: schedules
//    }
    Rectangle {
        id:schedulesListContainer
        anchors.top: headline.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.bottom:acceptButton.top
        anchors.bottomMargin: 10
        width: 360;
        ListView {
            id:schedulesList
            anchors.fill: parent
            model: endpoint.schedules
            delegate: schedulesDelegate
            clip:true
        }
        Component {
            id: schedulesDelegate
            Rectangle {
                id:scheduleDelegateBackground
                width:360; height: 40
                property string colorIdle: ((index %2 ==0) ? "#333333" : "#595959")
                color: delegateMouseArea.pressed ? "#0059b3": schedulesList.currentIndex == index ? "#003366": colorIdle
                Text{
                    id:onTimeText
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    leftPadding: 10
                    text:"EIN " + modelData.eventStartTime.toLocaleTimeString(Qt.locale("de_DE"),"h:mm") + "Uhr"
                    font.pixelSize:15
                }
                Text{
                    id:offTimeText
                    anchors.left: onTimeText.right
                    anchors.leftMargin: 10
                    leftPadding:10
                    text:"AUS " + modelData.eventEndTime.toLocaleTimeString(Qt.locale("de_DE"),"h:mm") + "Uhr"
                    font.pixelSize:15
                }
                MouseArea {
                    id:delegateMouseArea
                    anchors.fill:scheduleDelegateBackground
                    onClicked:{
                        schedulesList.currentIndex = index
                        scheduleDialog.schedule = modelData
                        scheduleDialog.show()
                        console.log("onclicked delegate")
                    }
                }
                Text {
                    anchors.right: parent.right
                    text:"löschen"
                    font.bold: true
                    font.pixelSize: 15
                    color: deleteScheduleMouseArea.pressed ? "brightRed": "darkRed"
                    MouseArea {
                        id:deleteScheduleMouseArea
                        anchors.fill: parent
                        onClicked: endpoint.removeSchedule(modelData)
                    }
                }
            }
        }
    }

    Image {
        id:addScheduleImage
        anchors.top: headline.bottom
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 150
        width:40; height:width
        source:"qrc:/images/images/add-square.png"
        MouseArea{
            id:addScheduleMouseArea
            anchors.fill:parent
            onClicked:{
                scheduleDialog.reset()
                scheduleDialog.show()
            }
        }
    }

    //BottomBar
    FrameButton {
        id:acceptButton
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 20
        text: "OK"
        onSignalClicked: {scheduleDialog.hide()
                         hide()}
    }


    ScheduleDialog {
        id:scheduleDialog
        visible:false
        opacity: 0
        width:600
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        endpoint:root.endpoint
    }
}

