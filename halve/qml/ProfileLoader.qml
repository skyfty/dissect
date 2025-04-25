import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts

Item {
    id:root
    property var profile:ProfileManager.currentProfile

    Component.onCompleted: {
        root.profile.increaseRef();
    }

    Component.onDestruction: {
        root.profile.decreasecRef();
    }

    StorageChecker {
        id:storageCheck
        threshold:AppSettings.storageThreshold
        profile:root.profile
        onProfileChanged: {
            storageCheck.launch();
        }

        onStateChanged: {
            if (storageCheck.state === Halve.SS_DANGER){
                currentChannel.disconnect();
                stackView.replace(storageDangerComponent);
            }
            root.profile.storageState = storageCheck.state;
        }
    }

    Connections {
        target: root.profile
        function onOpenSnapshot(snapshot) {
            var profileSnapshot = root.profile.loadSnapshot(snapshot);
            if (profileSnapshot) {
                stackView.replace(snapshotStudyLoadComponent, {profileSnapshot:profileSnapshot});
            }
        }
    }

    Channel {
        id:currentChannel
        profilePath:root.profile.path
        trackRate: AppSettings.catheterTrackRate
        mode: root.profile.channelMode
        port:AppSettings.channelPort   
        onPortChanged: {
            currentChannel.launch()
        }
        onModeChanged: {
            currentChannel.launch()
        }
        onStateChanged: {
            if (currentChannel.state === ChannelReplica.State_Ready) {
                currentChannel.connect();
            }
        }
        Component.onDestruction:{
            currentChannel.disconnect();
        }
    }


    Component {
        id:storageDangerComponent
        Rectangle {
            anchors.fill: parent
            color:"#f73636"
            Image {
                source: "qrc:/assets/images/error.png"
                width:700
                height: 700
                opacity:0.1
                anchors.centerIn: parent

            }
            Label {
                id:label
                font.pixelSize: 30
                font.weight: Font.Bold
                text: qsTr("Storage space is running out, please clean up the storage space in time.")
                anchors.centerIn: parent
            }
        }
    }

    Component {
        id:snapshotStudyLoadComponent
        Rectangle {
            required property var profileSnapshot

            color:"#21233a"
            Image {
                source: "qrc:/assets/images/microscope.png"
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
                    text:qsTr("Loading the snapshot environment...")
                    Layout.alignment: Qt.AlignHCenter
                    font.pixelSize: 20
                    font.weight: Font.Bold
                }
            }
            Connections {
                target: root.profile
                function onModelRefChanged(cnt){
                    if (cnt === 1) {
                        snapshotStudyLoadDelayTimer.start()
                    }
                }
            }

            Timer {
                id:snapshotStudyLoadDelayTimer
                interval: 100
                repeat: false
                onTriggered: {
                    stackView.replace(snapshotStudyComponent, {profileSnapshot:profileSnapshot});
                }
            }
        }
    }

    Component {
        id:studyCombackComponent
        Rectangle {
            required property var profileSnapshot
            color:"#21233a"
            Image {
                source: "qrc:/assets/images/microscope.png"
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
                    text:qsTr("Loading the experimental environment...")
                    Layout.alignment: Qt.AlignHCenter
                    font.pixelSize: 20
                    font.weight: Font.Bold
                }
            }
            Connections {
                target: profileSnapshot
                function onModelRefChanged(cnt){
                    if (cnt === 0) {
                        studyCombackDelayTimer.start();
                    }
                }
            }

            Timer {
                id:studyCombackDelayTimer
                interval: 100
                repeat: false
                onTriggered: {
                    currentChannel.reconnect();
                    stackView.replace(studyComponent);
                    root.profile.freeSnapshot(profileSnapshot);
                }
            }
        }
    }

    Component {
        id:snapshotStudyComponent
        Item {
            required property var profileSnapshot
            Connections {
                target: profileSnapshot
                function onClose() {
                    stackView.replace(studyCombackComponent, {profileSnapshot:profileSnapshot});
                }
            }

            StudyPage{
                anchors.fill: parent
                profile: profileSnapshot
                channel: currentChannel
            }
        }
    }

    Component {
        id:studyComponent
        StudyPage{
           profile: root.profile
           channel: currentChannel
       }
    }


    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: studyComponent
        onCurrentItemChanged: {
            if (currentItem) {
              currentItem.forceActiveFocus()
            }
        }

        replaceEnter:Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to:1
                duration: 100
            }
        }

        replaceExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to:0
                duration: 100
            }
        }
    }


}
