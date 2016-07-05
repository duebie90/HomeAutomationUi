import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Extras 1.4

Item{
    id:root
    visible:true
    opacity: 0
    width:400
    height: 300
    property variant endpoint:undefined
    //if schedule is undefined: this dialog is intendet to create a new one
    property variant schedule:undefined
    property bool weekdaysEnabled: true//intervallCombo.currentIndex !=4
    onScheduleChanged: {
        console.log("ScheduleDialog schedule set rep type = "+  schedule.repetition)
        intervallCombo.currentIndex = schedule.repetition
        hoursTextFieldStart.text = schedule.eventStartTime.toLocaleTimeString(Qt.locale("de_DE"),"hh")
        minutesTextFieldStart.text = schedule.eventStartTime.toLocaleTimeString(Qt.locale("de_DE"),"mm")
        hoursTextFieldEnd.text = schedule.eventEndTime.toLocaleTimeString(Qt.locale("de_DE"),"hh")
        minutesTextFieldEnd.text = schedule.eventEndTime.toLocaleTimeString(Qt.locale("de_DE"),"mm")
        console.log("ScheduleDialog weekdays[0]=" + schedule.weekdays[0])
        monday.checked = schedule.weekdays[0]
        tuesday.checked = schedule.weekdays[1]
        wednesday.checked = schedule.weekdays[2]
        thursday.checked = schedule.weekdays[3]
        friday.checked = schedule.weekdays[4]
        saturday.checked = schedule.weekdays[5]
        sunday.checked = schedule.weekdays[6]
    }

    Rectangle{
        id:background
        anchors.fill: parent
        //color:"#0d0d0d"
        color: "#262626"
        border.color: "white"
        border.width: 1
    }
    MouseArea {
        anchors.fill: parent
        //MouseArea to catch clicks onto underlying components
    }
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
    function reset(){resetWeekdays()
                    hoursTextFieldStart.text    = hoursTextFieldStart.placeholderText
                    minutesTextFieldStart.text  = minutesTextFieldStart.placeholderText;
                    hoursTextFieldEnd.text      = hoursTextFieldEnd.placeholderText
                    minutesTextFieldEnd.text    = minutesTextFieldEnd.placeholderText
                    root.schedule = undefined}
    function resetWeekdays() {
                            monday.checked=false;
                            tuesday.checked=false;
                            wednesday.checked=false;
                            thursday.checked=false;
                            friday.checked=false;
                            saturday.checked=false;
                            sunday.checked=false;
    }

    Text{
        id:headlineWeekdays
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: 20
        color:"#a9a9a9"
        text: "Wochentage auswählen, an den der Zeitplan gelten soll"
    }
    Rectangle{
        id:weekdaysContainer
        color:"transparent"
        anchors.top:headlineWeekdays.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width:400; height:40
        CheckBox {
            id:monday
            anchors.left: parent.left
            checked:false
            enabled: weekdaysEnabled
            onCheckedChanged:{endpoint.weekdayChecked(0, checked); if(checked==true) intervallCombo.currentIndex = 4}
            text:"<font color=\"#a9a9a9\" size=\"30px\">Mo</font>"
        }
        CheckBox {
            id:tuesday
            anchors.left: monday.right
            checked:false
            enabled: weekdaysEnabled
            onCheckedChanged:{endpoint.weekdayChecked(1, checked); if(checked==true) intervallCombo.currentIndex = 4}
            text:"<font color=\"#a9a9a9\" size=\"30px\">Di</font>"
        }
        CheckBox {
            id:wednesday
            anchors.left: tuesday.right
            checked:false
            enabled: weekdaysEnabled
            onCheckedChanged:{endpoint.weekdayChecked(2, checked); if(checked==true)intervallCombo.currentIndex = 4}
            text:"<font color=\"#a9a9a9\" size=\"30px\">Mi</font>"
        }
        CheckBox {
            id:thursday
            anchors.left: wednesday.right
            checked:false
            enabled: weekdaysEnabled
            onCheckedChanged:{endpoint.weekdayChecked(3, checked); if(checked==true)intervallCombo.currentIndex = 4}
            text:"<font color=\"#a9a9a9\" size=\"30px\">Do</font>"
        }
        CheckBox {
            id:friday
            anchors.left: thursday.right
            checked:false
            enabled: weekdaysEnabled
            onCheckedChanged:{endpoint.weekdayChecked(4, checked); if(checked==true) intervallCombo.currentIndex = 4}
            text:"<font color=\"#a9a9a9\" size=\"30px\">Fr</font>"
        }
        CheckBox {
            id:saturday
            anchors.left: friday.right
            checked:false
            enabled: weekdaysEnabled
            onCheckedChanged:{endpoint.weekdayChecked(5, checked); if(checked==true)intervallCombo.currentIndex = 4}
            text:"<font color=\"#a9a9a9\" size=\"30px\">Sa</font>"
        }
        CheckBox {
            id:sunday
            anchors.left: saturday.right
            checked:false
            enabled: weekdaysEnabled
            onCheckedChanged:{ endpoint.weekdayChecked(6, checked); if(checked==true) intervallCombo.currentIndex = 4}
            text: "<font color=\"#a9a9a9\" size=\"30px\">So</font>"
        }

    }
    Rectangle {
        id:starttimeContainer
        width:starttimeLabel.width + hoursTextFieldStart.width + delimiterPointsStart.width + minutesTextFieldStart.width
        height:hoursTextFieldStart.height
        color:"transparent"
        anchors.top: weekdaysContainer.bottom
        anchors.topMargin: 5
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 20
        Text {
            id:starttimeLabel
            anchors.top:starttimeContainer.top
            anchors.topMargin: 5
            anchors.left: parent.left
            text:"EINSCHALTZEIT"
            font.pixelSize: 20
            color:"#a9a9a9"
        }
        TextField {
            id:hoursTextFieldStart
            anchors.top:starttimeContainer.top
            anchors.left: starttimeLabel.right
            anchors.leftMargin: 5
            maximumLength: 2
            width:50
            placeholderText: "10"
            color: "white"
            validator: IntValidator{bottom:0; top:23}
            onActiveFocusChanged: if(activeFocus) {text=""}
        }
        Text {
            id:delimiterPointsStart
            anchors.top:starttimeContainer.top
            anchors.left: hoursTextFieldStart.right
            anchors.leftMargin: 2
            anchors.verticalCenter: parent.verticalCenter
            text:":"
            font.pixelSize:30
            color: "white"
        }
        TextField {
            id:minutesTextFieldStart
            anchors.top:starttimeContainer.top
            anchors.left: delimiterPointsStart.right
            anchors.leftMargin: 2
            maximumLength:  2
            width:50
            placeholderText: "00"
            color: "white"
            validator: IntValidator{bottom:0; top:59}
            onActiveFocusChanged: if(activeFocus) {text=""}
        }
    }
    Rectangle {
        id:endtimeContainer
        width:endtimeLabel.width + hoursTextFieldEnd.width + delimiterPointsEnd.width + minutesTextFieldEnd.width
        height:hoursTextFieldEnd.height
        color:"transparent"
        anchors.top: weekdaysContainer.bottom
        anchors.topMargin: 5
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 20
        Text {
            id:endtimeLabel
            anchors.top:endtimeContainer.top
            anchors.topMargin:5
            anchors.left: parent.left
            text:"AUSSCHALTZEIT"
            font.pixelSize: 20
            color:"#a9a9a9"
        }
        TextField {
            id:hoursTextFieldEnd
            anchors.top:endtimeContainer.top
            anchors.left: endtimeLabel.right
            anchors.leftMargin: 5
            maximumLength: 2
            width:50
            placeholderText: "11"
            color: "white"
            validator: IntValidator{bottom:0; top:23}
            onActiveFocusChanged: if(activeFocus) {text=""}
        }
        Text {
            id:delimiterPointsEnd
            anchors.top:endtimeContainer.top
            anchors.left: hoursTextFieldEnd.right
            anchors.leftMargin: 2
            anchors.verticalCenter: parent.verticalCenter
            text:":"
            font.pixelSize:30
            color: "white"
        }
        TextField {
            id:minutesTextFieldEnd
            anchors.top:endtimeContainer.top
            anchors.left: delimiterPointsEnd.right
            anchors.leftMargin: 2
            maximumLength:  2
            width:50
            placeholderText: "30"
            color: "white"
            validator: IntValidator{bottom:0; top:59}
            onActiveFocusChanged: if(activeFocus) {text=""}
        }
    }
    Text{
        id:intervallLabel
        anchors.top: endtimeContainer.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        width:paintedWidth
        text:"Widerholungsintervall wählen"
        font.pixelSize: 20
        color:"#a9a9a9"
    }

    ComboBox {
        id:intervallCombo
        anchors.top: intervallLabel.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        width: 200
        model: ["keine Wdh.", "täglich", "werktags", "Samstag & Sonntag", "wöchentlich"]
        onCurrentIndexChanged:{ endpoint.scheduleIntervallChosen(currentIndex)
            if(currentIndex != 4) {
                resetWeekdays()
            }

        }
    }
    FrameButton {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 20
        text: root.schedule == undefined ? "erstellen": "speichern"
        onSignalClicked:{
            var startTimeString = hoursTextFieldStart.text + minutesTextFieldStart.text;
            var endTimeString   = hoursTextFieldEnd.text + minutesTextFieldEnd.text;
            if(root.schedule == undefined) {
                endpoint.saveNewSchedule(startTimeString, endTimeString);
            }
            else {
                endpoint.saveScheduleChanges(root.schedule, startTimeString, endTimeString, intervallCombo.currentIndex);
            }

            root.hide()
            root.schedule = undefined
            root.reset()
        }
    }
    FrameButton {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 20
        text: "Abbrechen"
        onSignalClicked: {  root.hide()
                            root.schedule = undefined
                            root.reset()}
    }
}
