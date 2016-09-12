import QtQuick 2.0

Item {
    id:root
    width:100
    height:600
    signal signalCurrentItemChanged(int currentItemIndex)
    property int selectedtIndex: 0
    Rectangle {
        anchors.fill: parent
        color:"#262626"
    }
    ListView {
        anchors.fill: parent
        model: overviewScreen.endpoints
        delegate: Rectangle{
            width:root.width
            height:50
            property bool selected: index === selectedEndpointIndex
            color: "#262626"
            border.color: selected ? "#336699" : "grey"
            border.width: 3
            Text {
                anchors.left: parent.left
                anchors.leftMargin: 5
                id:endpointNameText
                text:modelData.name
                font.pixelSize: 15
                font.bold:false
                color: "#a9a9a9"
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top:endpointNameText.bottom
                anchors.topMargin: 5
                text:modelData.connected ? "online" : "offline"
                color:modelData.connected ? "darkGreen" : "darkRed"
                font.pixelSize: 10
                font.italic: true
            }
            Rectangle {
                id:switchedOnCircle
                anchors.right: parent.right
                anchors.rightMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                height:20
                width:20
                radius:10
                color: modelData.switchedState ? "green" : "#262626"
                border.color: "#a9a9a9"
                border.width: 2

            }
            MouseArea {
                id:delegateMouseArea
                anchors.fill: parent
                onPressed:{
                    signalCurrentItemChanged(index)
                    root.selectedtIndex = index
                }
            }
        }
    }
}
