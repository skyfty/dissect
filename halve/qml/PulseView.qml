import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve
Item {
    id:root
    required property var profile
    property alias channel:pulse.channel
    property var meltSamplingElectrode : root.profile.meltOptions.meltSamplingElectrode
    property var catheterListModel
    property var setting: root.profile.meltSetting

    BodySurfaceElectrodeNodeListModel{
        id:bodySurfaceElectrodeNodeListModel
        profile:root.profile
    }

    Timer {
        interval: 500; running: true; repeat: true
        onTriggered: pulse.update()
    }
    Pulse {
        id:pulse
        samplingRate:AppSettings.samplingRate
        profile:root.profile
    }

    function setPulseElectrodeIdComboBox() {
        pulseElectrodeIdComboBox.currentIndex = pulseElectrodeIdComboBox.indexOfValue(root.profile.pulseElectrodeId)
        pulseElectrodeIdComboBox.displayText = bodySurfaceElectrodeNodeListModel.data(root.profile.pulseElectrodeId, CatheterListModel.Name);
    }

    RowLayout {
        id:toolbar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 3
        ComboBox {
            id:pulseElectrodeIdComboBox
            Layout.maximumWidth: 120
            Layout.fillHeight: true
            enabled: setting.consultElectrodeId === ""
            Component.onCompleted:setPulseElectrodeIdComboBox()
            onActivated:{
                root.profile.pulseElectrodeId =currentValue
            }

            Connections {
                target: root.profile
                function onPulseElectrodeIdChanged(){
                    setPulseElectrodeIdComboBox();
                }
            }

            textRole: "name"
            valueRole: "id"
            Layout.fillWidth: true
            model: bodySurfaceElectrodeNodeListModel
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    Item {
        anchors.top: toolbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom


        RowLayout {
            anchors.fill: parent
            Layout.alignment: Qt.AlignVCenter
            spacing: 40

            Column {
                Layout.alignment: Qt.AlignHCenter
                Label {
                    text:qsTr("HR(sub/min)")
                    color:"white"
                    font.pixelSize: 20
                    font.bold: true
                    width: 130
                    horizontalAlignment: Text.AlignHCenter
                }

                Label {
                    text:root.profile.pulse
                    color:"#4eff00"
                    font.bold: true
                    font.pixelSize: 35
                    width: 130
                    horizontalAlignment: Text.AlignHCenter
                }

                Rectangle {
                    width:130
                    height: 1
                    color:"white"
                }
            }
            Column {
                Layout.alignment: Qt.AlignHCenter
                visible: setting.consultElectrodeId !== ""
                Label {
                    text:qsTr("CL(millisecond)")
                    color:"white"
                    font.pixelSize: 20
                    font.bold: true
                    width: 110
                    horizontalAlignment: Text.AlignHCenter
                }

                Label {
                    text:Math.floor(root.profile.latPerimeter / AppSettings.samplingRate*1000)
                    color:"#4eff00"
                    font.bold: true
                    font.pixelSize: 35
                    width: 110
                    horizontalAlignment: Text.AlignHCenter
                }

                Rectangle {
                    width:110
                    height: 1
                    color:"white"
                }
            }

            // Item {
            //     Layout.fillHeight: true
            //     Layout.fillWidth: true

            // }

        }
    }
}
