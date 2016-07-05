import QtQuick 2.0

Item {
    id:root
    objectName: "EndpointOverviewScreen"
    property variant selectedEndpoint: undefined
    property int selectedEndpointIndex: 0
    signal signalShownEndpointChanged(int index)
    Rectangle {
        id:background
        anchors.fill: parent
        color:"black"
    }

//all endpoint-screens are generated here
//only one is shown at the same time
   Repeater {
       id:endpointWidgetsList
       model: endpoints
       delegate: EndpointWidget {
            endpoint: modelData
            height:root.height
            width:600
            defaultX: 200
            shown: selectedEndpointIndex === index
            x: shown ? 200 : 200-width
        }
   }
   SideBarEndpointList {
       id:sidebar
       width:200
       anchors.top: parent.top
       anchors.left: parent.left
       anchors.bottom: parent.bottom
       onSignalCurrentItemChanged:{
           console.log("EOS signalCurrentItemChanged + name" + currentItemIndex)
           //selectedEndpoint = currentItem
           selectedEndpointIndex = currentItemIndex
           signalShownEndpointChanged(currentItemIndex)
       }
   }
}
