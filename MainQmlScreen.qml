import QtQuick 2.0

Item {
    id:root
    signal signalQmlLoaded()
    Rectangle {
        id:background
        anchors.fill: parent
        color:"black"
    }
    Loader {
        id:mainContent
        anchors.fill: parent
        source:mainContentSource
        onLoaded: signalQmlLoaded()
    }
}
