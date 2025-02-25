
import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve 1.0
import Qt.labs.platform as Platform

ColumnLayout {
    id:root
    required  property var magnetismAmount
    required property int consult
    required property real consultDistance
    required property int target
    required property real targetDistance
    property alias errorText:label.text
    Label {
        id:label
        visible: text !== ""
        color: "red"
    }

    function checkValid() {
        if (root.target  === root.consult ) {
            label.text = qsTr("The reference electrode and the target electrode cannot be the same");
            return false;
        }
        label.text = "";
        return true;
    }

    RowLayout {
        GroupBox {
            focusPolicy: Qt.ClickFocus
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            ColumnLayout {
                anchors.fill: parent
                ColumnLayout {
                    Label {
                        text: qsTr("Consult")
                        Layout.fillWidth: true
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        ComboBox {
                            id:consultComboBox
                            Layout.fillWidth: true
                            displayText:root.consult === -1 ? qsTr("Select"): root.consult + 1

                            popup: Menu {
                                width: consultComboBox.width
                                topMargin: 6
                                bottomMargin: 6
                                padding: 1
                                height: Math.min(contentItem.implicitHeight + 2, consultComboBox.Window.height - topMargin - bottomMargin)
                                Repeater {
                                    model:root.magnetismAmount
                                    MenuItem {
                                        id:consultmenuItem
                                        text:modelData + 1
                                        font.bold: root.consult === modelData
                                        onTriggered: {
                                            root.consult = modelData
                                            checkValid()
                                        }
                                        height: 30
                                        background: Rectangle {
                                            color: "#0078d4";
                                            visible: consultmenuItem.hovered
                                        }
                                        hoverEnabled: true
                                    }
                                }
                            }
                        }

                    }
                }
                ColumnLayout {
                    Label {
                        text: qsTr("Consult Distance (mm)")
                        Layout.fillWidth: true
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        DoubleSpinBox {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 25
                            value: root.consultDistance
                            from: -20.00
                            to: 20.00
                            stepSize: 0.01
                            editable: true
                            onValueModified: {
                                root.consultDistance = value
                            }
                        }

                    }
                }
            }

        }
        GroupBox {
            focusPolicy: Qt.ClickFocus
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            ColumnLayout {
                anchors.fill: parent
                ColumnLayout {
                    Label {
                        text: qsTr("Target")
                        Layout.fillWidth: true
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        ComboBox {
                            id:targetComboBox

                            displayText:root.target === -1 ?qsTr("Select"): root.target + 1

                            popup: Menu {
                                width: targetComboBox.width
                                topMargin: 6
                                bottomMargin: 6
                                padding: 1
                                height: Math.min(contentItem.implicitHeight + 2, targetComboBox.Window.height - topMargin - bottomMargin)

                                Repeater {
                                    model:root.magnetismAmount

                                    MenuItem {
                                        id:targetmenuItem
                                        text:modelData + 1
                                        font.bold: root.target === modelData
                                        onTriggered: {
                                            root.target = modelData;

                                            checkValid();
                                        }
                                        height: 30
                                        background: Rectangle {
                                            color: "#0078d4";
                                            visible: targetmenuItem.hovered
                                        }
                                        hoverEnabled: true
                                    }
                                }
                            }
                            Layout.fillWidth: true
                        }

                    }
                }
                ColumnLayout {
                    Label {
                        text: qsTr("Target Distance (mm)")
                        Layout.fillWidth: true
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        DoubleSpinBox {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 25
                            value: root.targetDistance
                            from: -20.00
                            to: 20.00
                            stepSize: 0.01
                            editable: true
                            onValueModified: {
                                root.targetDistance = value
                            }
                        }

                    }
                }
            }

        }
    }
}

