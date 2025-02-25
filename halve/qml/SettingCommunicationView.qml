import QtQuick
import QtCore
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtQuick.Controls
import "models"

Rectangle{
    anchors.fill: parent
    radius:10
    anchors.margins: 10
    color:"#3d3d3d"
    clip: true
    Item {
        anchors.fill: parent
        anchors.margins: 10
        GridLayout {
            columns: 2
            rowSpacing:8
            columnSpacing:8
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            Label {
                text: qsTr("Channel service port")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }
            IntSpinBox {
                id:channelPort
                from: 1000
                to: 65535
                value:AppSettings.channelPort
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Interactive Control Port")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }
            IntSpinBox {
                id:controlPort
                from: 1000
                to: 65535
                Layout.alignment: Qt.AlignLeft
                value:DevelopSettings.controlPort
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("FPGA Port")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }
            Row {
                TextField {
                   id:fpgaAddress
                   placeholderText: "IP Address"
                   placeholderTextColor: "gray"
                   text:DevelopSettings.fpgaAddress
               }

                TextField {
                    id:fpgaPort
                    validator: IntValidator{bottom: 1000; top: 65535;}
                    inputMethodHints: Qt.ImhDigitsOnly
                    Layout.alignment: Qt.AlignLeft
                    text:DevelopSettings.fpgaPort
                    width: 40
                }
            }
            Label {
                text: qsTr("NDI receiving frequency")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }
            IntSpinBox {
                id:combinedIntervalField
                from: 0
                to: 60000
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.combinedInterval
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Body Surface Waveform Range")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            DoubleSpinBox {
                id:ecgBasisField
                from: 0.00
                to: 10000.00
                editable:true
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.ecgBasis
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("In-body waveform range")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            DoubleSpinBox {
                id:basisField
                from: 0.00
                to: 500.00
                editable:true
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.basis
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Sampling rate")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            IntSpinBox {
                id:samplingRateField
                from: 0
                to: 4096
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.samplingRate
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Upload Frequency")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            IntSpinBox {
                id:uploadRateField
                from: 0
                to: 4096
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.uploadRate
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Save NDI data")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }
            CheckBox {
                id:keepSave
                Layout.alignment: Qt.AlignLeft
                checked:AppSettings.keepSave
            }


            Label {
                text: qsTr("Transformation matrix frequency (Second)")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            IntSpinBox {
                id:magnetismTrainRate
                from: 0
                to: 30
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.magnetismTrainRate
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Electric field conversion coefficient")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }


            ColumnLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignLeft
                RowLayout {
                    Label {
                        text: "X:"
                        font.pixelSize: 14
                        Layout.alignment: Qt.AlignRight
                    }

                    DoubleSpinBox {
                        id:coefficientX
                        from: 1.00
                        to: 100.00
                        editable:true
                        Layout.alignment: Qt.AlignLeft
                        value: AppSettings.coefficient.x
                        Layout.fillWidth: true
                        onValueModified: {
                            AppSettings.coefficient = Qt.vector3d(coefficientX.value, coefficientY.value, coefficientZ.value)
                        }
                    }
                }

                RowLayout {
                    Label {
                        text: "Y:"
                        font.pixelSize: 14
                        Layout.alignment: Qt.AlignRight
                    }

                    DoubleSpinBox {
                        id:coefficientY
                        from: 1.00
                        to: 100.00
                        editable:true
                        Layout.alignment: Qt.AlignLeft
                        value: AppSettings.coefficient.y
                        Layout.fillWidth: true
                        onValueModified: {
                            AppSettings.coefficient = Qt.vector3d(coefficientX.value, coefficientY.value, coefficientZ.value)
                        }

                    }
                }
                RowLayout {
                    Label {
                        text: "Z:"
                        font.pixelSize: 14
                        Layout.alignment: Qt.AlignRight
                    }

                    DoubleSpinBox {
                        id:coefficientZ
                        from: 1.00
                        to: 100.00
                        editable:true
                        Layout.alignment: Qt.AlignLeft
                        value: AppSettings.coefficient.z
                        Layout.fillWidth: true
                        onValueModified: {
                            AppSettings.coefficient = Qt.vector3d(coefficientX.value, coefficientY.value, coefficientZ.value)
                        }
                    }
                }
            }
        }
    }

    Component.onDestruction: {
        AppSettings.basis = basisField.value
        AppSettings.ecgBasis = ecgBasisField.value
        AppSettings.samplingRate = samplingRateField.value
        AppSettings.uploadRate = uploadRateField.value
        AppSettings.combinedInterval = combinedIntervalField.value
        DevelopSettings.fpgaAddress = fpgaAddress.text
        DevelopSettings.fpgaPort = fpgaPort.text
        DevelopSettings.controlPort = controlPort.value
        AppSettings.channelPort = channelPort.value
        AppSettings.keepSave = keepSave.checked
        AppSettings.magnetismTrainRate = magnetismTrainRate.value
    }
}
