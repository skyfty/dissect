import QtQuick
import QtCore
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import "utils.js" as Utils

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
                text: qsTr("Language")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }
            ComboBox {
                Component.onCompleted: currentIndex = indexOfValue(InterfaceSettings.language)
                onActivated: {
                    InterfaceSettings.language = currentValue
                    Translator.reload(InterfaceSettings.language);
                }
                textRole: "text"
                valueRole: "value"
                Layout.alignment: Qt.AlignLeft
                model: ListModel {
                    ListElement {
                        value:"zh"
                        text: "简体中文"
                    }
                    ListElement {
                        value:"en"
                        text: "English"
                    }
                }
                Layout.fillWidth: true
            }

            Label {
                text:  qsTr("Default User Path")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
                visible:  ProfileManager.state === Halve.PMS_STRAINING
            }
            RowLayout {
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                visible:  ProfileManager.state === Halve.PMS_STRAINING
                TextField {
                    text: AppSettings.defaultUserPath
                    Layout.fillWidth: true
                    color: "white"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            folderDialogComponent.createObject(parent).open();
                        }
                    }

                    Component {
                        id:folderDialogComponent
                        FolderDialog {
                             id: folderDialog
                             currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
                             onAccepted: {
                                 var fileUrl = new String(folderDialog.selectedFolder);
                                 var filePath =  fileUrl.substring(8);
                                 if (filePath !== AppSettings.defaultUserPath) {
                                     AppSettings.defaultUserPath = filePath;
                                     Utils.showMessage(mainWindow, qsTr("The default user path has been set to the new path, and must be restarted to make the new path effective"));
                                 }
                             }
                        }
                    }

                }
            }
            Label {
                text:  qsTr("The ECG channel color")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }
            ColorCarp{
                id:ecgElectrodeColorPicker
                Layout.preferredHeight: 30
                Layout.alignment: Qt.AlignLeft
                color:InterfaceSettings.ecgElectrodeColor
                onAccepted: InterfaceSettings.ecgElectrodeColor =  ecgElectrodeColorPicker.color.toString()
                Layout.fillWidth: true
            }

            Label {
                text:  qsTr("Channel ruler step length")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            IntSpinBox {
                id:oscillograpScaleplateStepField
                from: 5
                 to: 60
                 value: AppSettings.oscillograpScaleplateStep
                 Layout.fillWidth: true
                 Layout.alignment: Qt.AlignLeft
            }


            Label {
                text: qsTr("Catheter refresh rate (Hz)")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            IntSpinBox {
                id:catheterTrackRate
                from: 1.00
                to: 20.00
                editable:true
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.catheterTrackRate
                Layout.fillWidth: true
                onValueChanged: {
                    AppSettings.catheterTrackRate = value
                }
            }
            Label {
                text: qsTr("Fully automated screen recording")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }
            CheckBox {
                id:autoOmnidistanceRecordScreenEnable
                Layout.alignment: Qt.AlignLeft
                checked:NotebookSettings.autoOmnidistanceRecordScreen
            }


            Label {
                text: qsTr("Event Recording")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            CheckBox {
                id:autoRecordScreen
                Layout.alignment: Qt.AlignLeft
                checked:NotebookSettings.autoRecordScreen
            }

            Label {
                text: qsTr("Event recording duration (seconds)")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }
            IntSpinBox {
                id:autoRecordDuration
                value: NotebookSettings.autoRecordDuration
                from:10
                to:30
                stepSize: 1
                enabled: autoRecordScreen.checked
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Interval screen recording")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }
            CheckBox {
                id:autoAlternationRecordScreen
                Layout.alignment: Qt.AlignLeft
                enabled: autoRecordScreen.checked
                checked:NotebookSettings.autoAlternationRecordScreen
            }


            Label {
                text: qsTr("Screen recording interval (minutes)")

                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            IntSpinBox {
                id:autoAlternationRecordScreenInterval
                value: NotebookSettings.autoAlternationRecordScreenInterval
                from:10
                to:30
                stepSize: 1
                enabled: autoAlternationRecordScreen.checked && autoRecordScreen.checked
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Reference Positioning Accuracy (mm)")

                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            DoubleSpinBox {
                id:consultPrecision
                from: 0.00
                to: 10.00
                editable:true
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.consultPrecision
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Breathing sampling frequency")

                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            IntSpinBox {
                id:breathSamplingRate
                from: 0
                to: 100
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.breathSamplingRate
                Layout.fillWidth: true
            }
            Label {
                text: qsTr("Breath Sampling Filtering")

                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            CheckBox {
                id:breathSamplingFilter
                Layout.alignment: Qt.AlignLeft
                checked:AppSettings.breathSamplingFilter
            }

            Label {
                text: qsTr("Minimum Effective Value of Breathing Samples")

                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            DoubleSpinBox {
                id:breathSamplingValidLimit
                from: 0.00
                to: 10.00
                editable:true
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.breathSamplingValidLimit
                Layout.fillWidth: true
            }


            Label {
                text: qsTr("Breathing Gate Coefficient")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            DoubleSpinBox {
                id:breathGatingRatio
                from: 0.00
                to: 100.00
                editable:true
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.breathGatingRatio
                Layout.fillWidth: true
                onValueModified: {
                    AppSettings.breathGatingRatio = breathGatingRatio.value
                }
            }


            Label {
                text: qsTr("Electric Center Shifting(mm)")
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
                        id:electricCenterX
                        from: -100.00
                        to: 100.00
                        editable:true
                        Layout.alignment: Qt.AlignLeft
                        value: -AppSettings.electricCenter.x
                        Layout.fillWidth: true
                        onValueModified: {
                            AppSettings.electricCenter = Qt.vector3d(-electricCenterX.value, electricCenterY.value, electricCenterZ.value)
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
                        id:electricCenterY
                        from: -100.00
                        to: 100.00
                        editable:true
                        Layout.alignment: Qt.AlignLeft
                        value: AppSettings.electricCenter.y
                        Layout.fillWidth: true
                        onValueModified: {
                            AppSettings.electricCenter = Qt.vector3d(-electricCenterX.value, electricCenterY.value, electricCenterZ.value)
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
                        id:electricCenterZ
                        from: -100.00
                        to: 100.00
                        editable:true
                        Layout.alignment: Qt.AlignLeft
                        value: AppSettings.electricCenter.z
                        Layout.fillWidth: true
                        onValueModified: {
                            AppSettings.electricCenter = Qt.vector3d(-electricCenterX.value, electricCenterY.value, electricCenterZ.value)
                        }
                    }
                }
            }

            Label {
                text: qsTr("Electric field reference displacement range(mm)")
                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }

            DoubleSpinBox {
                id:electricDisplacement
                from: 1.00
                to: 100.00
                editable:true
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.electricDisplacement
                Layout.fillWidth: true
            }



            Label {
                text: qsTr("Disk space warning(GB)")

                font.pixelSize: 14
                Layout.alignment: Qt.AlignRight
            }
            IntSpinBox {
                id:storageThreshold
                from: 1
                to: 100
                editable:true
                Layout.alignment: Qt.AlignLeft
                value: AppSettings.storageThreshold
                Layout.fillWidth: true
                onValueChanged: {
                    AppSettings.storageThreshold = value
                }
            }
        }
    }

    Component.onDestruction: {
        AppSettings.oscillograpScaleplateStep = oscillograpScaleplateStepField.value
        NotebookSettings.autoOmnidistanceRecordScreen = autoOmnidistanceRecordScreenEnable.checked
        NotebookSettings.autoAlternationRecordScreen = autoAlternationRecordScreen.checked
        NotebookSettings.autoAlternationRecordScreenInterval = autoAlternationRecordScreenInterval.value
        NotebookSettings.autoRecordScreen = autoRecordScreen.checked
        NotebookSettings.autoRecordDuration = autoRecordDuration.value
        AppSettings.consultPrecision = consultPrecision.value
        AppSettings.breathSamplingRate = breathSamplingRate.value
        AppSettings.breathSamplingFilter = breathSamplingFilter.checked
        AppSettings.breathSamplingValidLimit = breathSamplingValidLimit.value
        AppSettings.breathGatingRatio = breathGatingRatio.value
        AppSettings.electricDisplacement = electricDisplacement.value

    }

}
