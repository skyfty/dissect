import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

import Halve 1.0
import "models"

Item {
    id:root
    required property var profile
    property alias count:alarmListRepeater.count
    property alias interval:timer.interval
    property var backgroundOpacity: 0.6
    property var backgroundColor:"#333333"
    property bool loop: false
    property int currentIndex: 0

    AlarmModel {
        id:alarmModel
    }

    Timer {
        id:timer
        running: root.loop
        repeat: true
        onTriggered: {
            root.currentIndex++;
            if (root.currentIndex >= alarmListRepeater.count) {
                root.currentIndex = 0;
            }
        }
    }

    Connections {
        target: root.profile
        onStorageStateChanged: {
            switch(root.profile.storageState) {
            case Halve.SS_WARNING: {
                alarmListModel.add(Halve.AN_STORAGE_FULL_ERROR, Halve.AT_ERROR, true);
                break;
            }
            default: {
                alarmListModel.remove(Halve.AN_STORAGE_FULL_ERROR);
                break;
            }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        Repeater {
            id:alarmListRepeater
            model: AlarmListModel {
                id:alarmListModel
                profile: root.profile
            }

            AlarmPane {
                Layout.fillWidth: true
                Layout.minimumHeight:45
                required property int index
                required property var name
                required property var id
                required property var type
                text: alarmModel.getTextByValue(name)
                state: type
                backgroundColor:root.backgroundColor
                backgroundOpacity:root.backgroundOpacity
                visible: !root.loop || index === root.currentIndex
                onClose: {
                    alarmListModel.removeRows(index, 1)
                }
            }
        }
    }

}
