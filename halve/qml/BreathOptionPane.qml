
import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve 1.0
import "models"

ColumnLayout {
    id:root
    required property var profile
    property var breathOptions:profile.breathOptions
    property int timerInterval: 0
    required property var breathSurvey
    required property var combined
    required property var notebookDataSource
    readonly property int duration:12

    ColumnLayout {
        visible: root.combined.mode !== Halve.CHANNELMODE_ELECTRICAL
        Layout.fillWidth: true
        Layout.fillHeight: true
        ColumnLayout {
            Layout.topMargin:10
            Layout.bottomMargin: 10
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            Layout.fillWidth: true
            Label {
                text:qsTr("The catheter must be in a stable position\r\n for 12 seconds while collecting respiratory data")
            }
            Button {
                id:collectButton
                Layout.fillWidth: true
                enabled:root.combined.state === ChannelReplica.State_Tracking && !timer.running
                onClicked:  {
                    root.combined.channel.trackRate = 100;
                    root.profile.renovating = true;
                    root.notebookDataSource.autoRecord("CRD " + new Date().toLocaleString(locale, "hh:mm:ss"));
                }
                contentItem: Item {

                }

                ProgressBar {
                    id:progressBar
                    visible: timer.running
                    value: timerInterval / root.duration
                    width: parent.width
                    height: parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                RowLayout {
                    anchors.centerIn: parent
                    Label {
                        text:qsTr("Collect respiratory data")
                        color: "white"
                    }
                    Label {
                        text:(root.duration - timerInterval) + qsTr(" second")
                        visible: timer.running
                        color: "white"
                    }
                }

            }
            Label {
                visible: root.profile.pantSampling &&  !timer.running && breathOptions.breatheLimit < AppSettings.breathSamplingValidLimit
                text:qsTr("The breath sampling value is too small") + " " + breathOptions.breatheLimit.toFixed(2)
                color: "red"
                font.bold: true
            }
        }

        Timer {
            id:timer;
            repeat:true
            running: root.profile.renovating

            onRunningChanged: {
                timerInterval = 0;
            }

            triggeredOnStart:true
            onTriggered: {
                if (timerInterval++ >= root.duration) {
                    root.profile.renovating = false;
                    combined.channel.trackRate = AppSettings.catheterTrackRate;
                }
            }
        }

        ColumnLayout {
            Label {
                text: qsTr("Breathing boundary value coefficient (%)")
                Layout.fillWidth: true
            }

            Connections {
                target: root.breathOptions
                function onBreathBoundaryRatioChanged() {
                    breathBoundaryRatioSpinBox.value = root.breathOptions.breathBoundaryRatio;
                }
            }

            RowLayout {
                Layout.fillWidth: true

                DoubleSpinBox {
                    id:breathBoundaryRatioSpinBox
                    Layout.fillWidth: true
                    Layout.preferredHeight: 25
                    from: 0.0
                    to: 100
                    stepSize: 0.1
                    enabled:  !timer.running
                    value: root.breathOptions.breathBoundaryRatio
                    onValueModified: {
                        root.breathOptions.breathBoundaryRatio = value
                    }
                }
            }
        }
        ToolSeparator{
            Layout.fillWidth: true
            orientation: Qt.Horizontal
        }
    }



    ColumnLayout {
        spacing: 10
        Layout.leftMargin: 5
        CheckBox{
            id:enablecompensation
            text:qsTr("Enable breathing compensation")
            checked: root.breathOptions.enabledCompensate
            enabled: !timer.running
            onCheckedChanged:root.breathOptions.enabledCompensate = enablecompensation.checked
        }
    }

    ColumnLayout {
        id:breathingOptionWrap
        visible: root.breathOptions.enabledCompensate
        spacing: 10
        Layout.leftMargin: 20
        Layout.topMargin: 5

        ButtonGroup {
            id: breathingRadioGroup
        }

        RadioButton{
            id:breatheCompensationRadioButton
            ButtonGroup.group: breathingRadioGroup
            text:qsTr("Apply breathing compensation")
            checked: root.breathOptions.breatheCompensation
            enabled: !timer.running && root.breathOptions.enabledCompensate
            onCheckedChanged:root.breathOptions.breatheCompensation = checked
        }

        RadioButton{
            text:qsTr("Apply breathing gate")
            ButtonGroup.group: breathingRadioGroup
            checked: root.breathOptions.breatheGate
            enabled: !timer.running

            onCheckedChanged: {
                root.breathOptions.breatheGate = checked
            }

        }

    }
}
