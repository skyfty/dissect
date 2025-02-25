
import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve 1.0
import Qt.labs.platform as Platform

ColumnLayout {
    id:root
    required property var profile
    required property var dissolveOptions
    required property var catheter

    GroupBox {
        title: qsTr("Dissolve Electrode")
        Layout.fillWidth: true
        ColumnLayout {
            anchors.fill: parent
            ComboBox {
                id:electrodeComboBox
                Layout.fillHeight: true
                Layout.fillWidth: true

                popup: Menu {
                    id:electrodeMenu
                    width: electrodeComboBox.width
                    topMargin: 6
                    bottomMargin: 6
                    padding: 1
                    height: Math.min(contentItem.implicitHeight + 2, electrodeComboBox.Window.height - topMargin - bottomMargin)
                    Repeater {
                        model:catheter?catheter.amount:0
                        MenuItem {
                            required property int modelData
                            text: modelData + 1
                            font.bold: electrodeComboBox.displayText === text
                            onTriggered: {
                                electrodeComboBox.displayText = text
                                root.dissolveOptions.electrodeIndex = modelData;
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    electrodeComboBox.displayText = root.dissolveOptions.electrodeIndex + 1
                }
            }
        }
    }
}
