import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import "models"

ColumnLayout {
    id:root
    required property var setting
    required property var electrodeNodeListModel

    GroupBox {
        title: qsTr("Consult")
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        focusPolicy: Qt.ClickFocus

        ColumnLayout {
            anchors.fill: parent
            spacing:9

            ColumnLayout {
                Label{
                    text:qsTr("Source")
                    Layout.fillWidth: true
                }

                ComboBox {
                    id:consultElectrodeComboBox
                    Layout.fillWidth: true
                    displayText:root.setting.consultElectrodeId?electrodeNodeListModel.data(root.setting.consultElectrodeId, ElectrodeNodeSelectListModel.Name):consultElectrodeMenu.itemAt(0).text
                    popup: Menu {
                        id:consultElectrodeMenu
                        width: consultElectrodeComboBox.width
                        topMargin: 6
                        bottomMargin: 6
                        padding: 1
                        height: Math.min(contentItem.implicitHeight + 2, consultElectrodeComboBox.Window.height - topMargin - bottomMargin)
                        MenuItem {
                            text:qsTr("Please choose")
                            font.bold: consultElectrodeComboBox.displayText === text
                            onTriggered: {
                                root.setting.consultElectrodeId = null
                                root.setting.shiftChanged = true
                                consultElectrodeComboBox.displayText = text
                            }
                        }
                        Repeater {
                            model:electrodeNodeListModel
                            MenuItem {
                                required property int index
                                required property string name
                                required property var id
                                text:name
                                font.bold: consultElectrodeComboBox.displayText === text
                                onTriggered: {
                                    root.setting.consultElectrodeId = id
                                    root.setting.shiftChanged = true
                                    consultElectrodeComboBox.displayText = name
                                }
                            }
                        }
                    }

                }
            }
            ColumnLayout {
                Label{
                    text:qsTr("Perception")
                    Layout.fillWidth: true
                }
                ComboBox {
                    Component.onCompleted: currentIndex = indexOfValue(root.setting.consultPerception)
                    onActivated:{
                        root.setting.consultPerception = currentValue
                        root.setting.shiftChanged = true
                    }
                    textRole: "text"
                    valueRole: "value"
                    Layout.fillWidth: true
                    model: PerceptionModel{}
                }
            }
        }
    }

    GroupBox {
        title: qsTr("Sensitivity")
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        focusPolicy: Qt.ClickFocus
        ColumnLayout {
            anchors.fill: parent
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ButtonGroup {
                    id: sensitivityRadioGroup
                }
                RadioButton {
                    id:sensitivityAuto
                    checked: root.setting.autoSensitivity
                    text: qsTr("Auto")
                    ButtonGroup.group: sensitivityRadioGroup
                    onClicked:root.setting.autoSensitivity = true
                }
                RowLayout {
                    Layout.minimumWidth: 200

                    RadioButton {
                        id:sensitivityManual
                        text: qsTr("Set")
                        checked: !root.setting.autoSensitivity
                        ButtonGroup.group: sensitivityRadioGroup
                        onClicked:root.setting.autoSensitivity = false
                    }

                    IntSpinBox {
                        id:sensitivitySlider
                        Layout.fillWidth: true
                        from: 1
                        to: 100
                        stepSize:1
                        value: root.setting.sensitivity
                        enabled: sensitivityManual.checked
                        onValueModified: root.setting.sensitivity = value
                    }
                }
            }
        }
    }
}
