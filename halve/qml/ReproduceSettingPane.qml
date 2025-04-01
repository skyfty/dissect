
import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve 1.0
import Qt.labs.platform as Platform
import "models"


ColumnLayout {
    id:root
    required property var options
    required property var combined
    Connections {
        target: root.combined
        function onCenterPointChanged(){
            options.centerPoint = combined.centerPoint
        }
    }

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        ColumnLayout {
            Layout.preferredWidth: 150
            Label {
                text: qsTr("Smoothness")
                Layout.fillWidth: true
            }
            RowLayout {
                Layout.fillWidth: true

                IntSpinBox {
                    id:iterationsSlider
                    Layout.fillWidth: true
                    Layout.preferredHeight: 25
                    from: 8
                    stepSize: 1
                    value: options.iterations
                    to: 100
                    onValueModified: {
                        options.iterations = value
                    }
                }
            }
        }
        ColumnLayout {
            Layout.preferredWidth: 150
            Label {
                text: qsTr("Filling accuracy")
                Layout.fillWidth: true
            }
            RowLayout {
                Layout.fillWidth: true
                ComboBox {
                    id:kernelSizeSlider
                    Layout.fillWidth: true
                    Layout.preferredHeight: 25
                    textRole: "text"
                    valueRole: "value"
                    model: KernelSizeModel{}
                    onActivated: {
                        options.kernelSize = currentValue
                    }
                    Component.onCompleted:currentIndex = indexOfValue(options.kernelSize)
                }

            }
        }

    }


}
