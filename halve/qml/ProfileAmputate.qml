import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts

Item {
    id:root
    required property var profileListModel
    required property int index

    Connections {
        target: ProfileManager.currentProfile
        function onModelRefChanged(cnt){
            if (cnt === 0) {
                delayTimer.start();
            }
        }
    }

    Timer {
        id:delayTimer
        interval: 1000
        onTriggered: {
            ProfileManager.setCurrentProfile("test");
            root.profileListModel.removeRows(root.index, 1);
        }
    }

    Rectangle {
        anchors.fill: parent
        color:"#21233a"
        Image {
            source: "qrc:/assets/images/lajixiang.svg"
            width:700
            height: 700
            opacity:0.02
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }

        ColumnLayout {
            anchors.centerIn: parent
            BusyIndicator {
                Layout.alignment: Qt.AlignHCenter
                running: true
            }
            Label {
                id:lable
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: 20
                font.weight: Font.Bold
                text: qsTr("Clean up the experimental data...")
            }
        }
    }
}
