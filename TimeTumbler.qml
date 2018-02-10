import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Extras 1.4
Tumbler {
    anchors.left: parent.left
    anchors.top: weekdaysContainer.bottom
    anchors.bottomMargin: 5
    height: 100
    TumblerColumn {
         id:hour1
         model: [0, 1, 2]

    }
    TumblerColumn {
         id:hour0
         model: hour1.currentIndex !== 2 ? [0, 1, 2, 3, 4, 5, 6, 7, 8, 9] : [0, 1, 2, 3]
    }
    Text {
         text:":"
         font.pixelSize: 30
         color:"white"
         anchors.horizontalCenter: parent.horizontalCenter
    }
    TumblerColumn {
         id:minute1
         model: [0, 1, 2, 3, 4, 5]
    }
    TumblerColumn {
         id:minute0
         model: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]         
    }
}
