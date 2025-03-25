
import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve 1.0
import Qt.labs.platform as Platform

RowLayout {
    id:root
    required property var profile
    property var mapping:root.profile.mapping
    property var mappingSetting:root.profile.mappingSetting

    ColumnLayout {
        Layout.alignment: Qt.AlignTop
        ReseauListModel {
            id:reseauListModel
            profile:root.profile
            onRowsInserted: reseauComboBox.resetCurrentIndex()
            onRowsRemoved: reseauComboBox.resetCurrentIndex();
        }
        GroupBox {
            title:qsTr("Repeat point selection")
            Layout.fillWidth: true
            ColumnLayout {
                 anchors.fill: parent
                 ColumnLayout {
                     Label {
                         text: qsTr("Repeat the point calculation radius(mm)")
                         Layout.fillWidth: true
                     }
                     RowLayout {
                         Layout.fillWidth: true
                         DoubleSpinBox {
                             enabled: root.profile.pointValidCheck
                             Layout.fillWidth: true
                             Layout.preferredHeight: 25
                             value: root.mappingSetting.duplicateRadius
                             from: 1.00
                             to: 40.00
                             stepSize: 1.0
                             onValueModified: {
                                 root.mappingSetting.duplicateRadius = value
                             }
                         }

                     }
                     Layout.bottomMargin: 4
                 }
                 RadioButton {
                     text: qsTr("The last repeat point")
                     checked: root.mappingSetting.useDuplicatePoints === Halve.Last
                     onClicked:  root.mappingSetting.useDuplicatePoints = Halve.Last
                 }
                 RadioButton {
                     text: qsTr("The best copy")
                     checked: root.mappingSetting.useDuplicatePoints === Halve.Best
                     onClicked:  root.mappingSetting.useDuplicatePoints = Halve.Best
                 }
             }
        }

        GroupBox {
            title:qsTr("Projection spacing(mm)")
            Layout.fillWidth: true
            ColumnLayout {
                 anchors.fill: parent
                 DoubleSpinBox {
                     enabled: root.profile.pointValidCheck
                     Layout.fillWidth: true
                     Layout.preferredHeight: 25
                     from: 0.00
                     stepSize: 1.00
                     value: root.mappingSetting.gap
                     to: 40.00
                     onValueModified: {
                         root.mappingSetting.gap = value
                     }
                 }
             }
        }
        GroupBox {
            title: qsTr("Figure appearance")
            focusPolicy: Qt.ClickFocus
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            ColumnLayout {
                anchors.fill: parent
                ColumnLayout {
                    Label {
                        text: qsTr("Internal projection(mm)")
                        Layout.fillWidth: true
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        DoubleSpinBox {
                            enabled: root.profile.pointValidCheck
                            Layout.fillWidth: true
                            Layout.preferredHeight: 25
                            value: root.mappingSetting.insideThrow
                            from: 0.00
                            to: 20.00
                            stepSize: 0.5
                            onValueModified: {
                                root.mappingSetting.insideThrow = value
                            }
                        }

                    }
                }
                ColumnLayout {
                    Label {
                        text: qsTr("External projection(mm)")
                        Layout.fillWidth: true
                    }
                    RowLayout {
                        Layout.fillWidth: true

                        DoubleSpinBox {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 25
                            value: root.mappingSetting.outsideThrow
                            enabled: root.profile.pointValidCheck

                            from: 0.00
                            to: 20.00
                            stepSize: 0.5
                            onValueModified: {
                                root.mappingSetting.outsideThrow = value
                            }
                        }

                    }
                }
            }

        }

        GroupBox {
            focusPolicy: Qt.ClickFocus
            Layout.fillWidth: true
            ColumnLayout {
                anchors.fill: parent
                CheckBox {
                    checked: root.mapping.showMappingInvalid
                    text: qsTr("Display invalid test points")
                    onClicked:  root.mapping.showMappingInvalid = checked
                }
                CheckBox {
                    checked: root.mapping.showRepeatInvalid
                    text: qsTr("Display repeated invalid points")
                    onClicked:  root.mapping.showRepeatInvalid = checked
                }
            }
        }
    }


}

