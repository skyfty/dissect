import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve

import Qt.labs.qmlmodels
import Qt.labs.platform

Item {
    id:root
    property alias color: dyestuff.color
    property var name: name

    signal accepted();
    signal closed();

    ColumnLayout {
        anchors.fill: parent
        spacing:0
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing:2
            anchors.margins: 5
            Label {
                text: qsTr("Name")
            }
            TextField {
                id: name
                focus: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft | Qt.AlignBaseline
                placeholderText:  qsTr("Name")
            }

            Label {
                text: qsTr("Color")
            }

            ColorCarp{
                id:dyestuff
                Layout.preferredHeight: 30
                Layout.fillWidth: true
            }


        }

        RowLayout {
            Layout.fillWidth: true
            Button {
                id:okButton
                Layout.fillWidth: true
                text: qsTr("Save")
                implicitHeight: 20
                onClicked: {
                    accepted();
                }
                enabled: name.text.trim() !== ""
                background: Rectangle {
                    implicitWidth: 80
                    implicitHeight: 24
                    radius: 2
                    color: okButton.down ? "#31609b": "#6493ce";
                }
            }
            Button {
                id:noButton
                Layout.fillWidth: true
                text: qsTr("Close")
                implicitHeight: 20
                onClicked: {
                    closed();
                }
                background: Rectangle {
                    implicitWidth: 80
                    implicitHeight: 24
                    radius: 2
                    color: noButton.down ? "#31609b": noButton.hovered?"#6493ce":"#404040";
                }
            }
        }
    }


}
