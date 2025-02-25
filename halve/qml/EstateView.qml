import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve

Item {
    id:root
    required property var profile
    required property var reciever
    required property var breathSurvey

    RowLayout {
        anchors.fill: parent
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
            PulseView {
                anchors.fill: parent
                anchors.margins: 10
                reciever:root.reciever
            }
        }
    }

}
