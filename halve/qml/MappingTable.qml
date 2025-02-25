
import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import QtQuick as QtQ

Rectangle {
    id: root
    property alias model:listView.model
    property alias count:listView.count
    property int currentRowId:-1
    property var buttonGroup
    required property var mappingType
    readonly property int itemHeight: 30
    color:"#333333"

    function scrollTo(mappingPointId) {
        root.currentRowId = mappingPointId;
        var idx = model.rowOfId(root.currentRowId)
        if (idx >= 0 && idx < listView.count) {
            flickable.contentY = idx * itemHeight
            flickable.returnToBounds();
        }
    }

    function clearCurrentIndex() {
        root.currentRowId = -1;
    }

    Connections {
        target: model
        function onRowsRemoved(){
            clearCurrentIndex();
        }
        function onRowsInserted(){
            clearCurrentIndex();
        }
    }

    Flickable {
        anchors.fill: parent
        id: flickable
        clip: true
        contentWidth: root.width
        contentHeight: repeaterItem.height
        rebound: Transition {
             NumberAnimation {
                 properties: "x,y"
                 duration: 0
                 easing.type: Easing.OutBounce
             }
         }

        Column {
            id: repeaterItem
            width: flickable.width
             Repeater {
                 id: listView
                 delegate: Rectangle {
                     width: parent.width
                     height: root.itemHeight
                     color:root.currentRowId == id? "#46607c" : valid ? (overcome ? "#7F8992": (index % 2==0?"#2b2b2b":"#282828")):"#626264"
                     RowLayout {
                         anchors.fill: parent
                         anchors.margins: 5
                         CheckBox {
                             property var rowId: id
                             Layout.preferredWidth: 30
                             Layout.fillHeight: true
                             ButtonGroup.group: root.buttonGroup
                         }

                         Label {
                             text:time.toLocaleString(locale, "hh:mm:ss.") + time.getMilliseconds().toString().padStart(3, '0')
                             font.pixelSize: 14
                             color:"lightyellow"
                         }
                         Label {
                             text:name
                             font.pixelSize: 14
                         }
                         Item {
                             Layout.fillWidth: true
                         }

                         Label {
                             visible: root.mappingType === Halve.Voltage
                             color:flags === 0?"white":"yellow"
                             text:maxVoltage === -1?"???":(Math.max(maxVoltage - minVoltage, 0)/ AppSettings.basis).toFixed(1) + " mV "
                             font.pixelSize: 14
                         }
                         Label {
                             visible: root.mappingType === Halve.Lat
                             color:flags === 0?"white":"yellow"
                             text:latIndex === -1?"???":(Math.floor(lat / AppSettings.samplingRate * 1000) + " ms")
                             font.pixelSize: 14
                         }
                     }

                     QtQ.TapHandler {
                         onTapped: {
                             root.currentRowId = id;
                         }
                     }
                 }
             }
        }
    }
}
