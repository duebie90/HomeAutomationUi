import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0

Item {
    id:root
//    visible: shown
    //onVisibleChanged: console.log("on visible changed EPW")
    property int defaultX:200
    property variant endpoint: undefined
    property bool shown: false
    //if another slide was clicked before completed it is scheduled
    property bool hideScheduled: false
    property bool showScheduled: false
    property alias delegate: delegateLoader.item
    property string delegateSource: ""
    Component.onCompleted: {
        //root.visible = shown
        //x = shown ? defaultX - root.width: defaultX
        console.log("DelegateLoader root.width " + root.width)
        console.log("INIT endpoint.name " + endpoint.name + " x:" + x)
    }


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
            delegate.hideEditSchedulesScreen()
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
        color: root.delegateSource == "EndpointWidgetX.qml" ? "red" : "blue"
        border.color: "#a9a9a9"
        border.width: 1

    function setModelData() {
        var delegate =  delegateLoader.item
        delegate.endpoint = root.endpoint
        console.log("setModelData endpoint.alias" + root.endpoint.name)
        console.log("root.shown/ x :" + root.shown + "/ " + root.x)
        console.log("root.visible: " + root.visible)
        console.log("root.autoMode: " + root.endpoint.autoMode)
        console.log("root.influx_temp: " + root.endpoint.influx_temp)
    }

    Loader{
        id:delegateLoader
        anchors.fill: parent
        source: root.delegateSource
        onLoaded: {parent.setModelData()}
        visible: root.visible
    }

   }
}

