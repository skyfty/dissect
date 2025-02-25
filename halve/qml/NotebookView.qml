
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels 1.0
import QtQuick.Window
import "models"

Pane {
    id:root
    required property var profile
    required property var source
    property var options:profile.notebookOptions
    property var sortType:profile.notebookOptions.sortType

    padding: 1
    clip: true
    onSortTypeChanged: {
        stackView.replace(root.sortType === Halve.NOTEBOOK_SORT_CATEGORY?"NotebookTreeView.qml":"NotebookListView.qml", {profile:root.profile, source:root.source});

    }

    HotkeyAction {
        profile:root.profile
        name: "snapshot"
        onActivated: {
            if(root.source.snapping) {
                root.source.snapshot(new Date().getTime());
            } else {
                root.source.beginSnapshot(new Date().getTime());
            }
        }
    }
    Label {
        visible: root.profile.type === Profile.SNAPSHOT
        anchors.centerIn: parent
        font.pixelSize: 16
        font.bold: true
        text:qsTr("This windows is invalid")
    }
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        visible: root.profile.type !== Profile.SNAPSHOT
        RowLayout {
            Layout.fillWidth: true
            Layout.maximumHeight:  34
            Layout.alignment:Qt.AlignVCenter

            ComboBox {
                id:sortComboBox
                Layout.margins: 1
                Layout.fillWidth: true
                textRole: "text"
                valueRole: "value"
                focusPolicy:Qt.NoFocus
                model: NotebookSortModel{}
                Component.onCompleted:{
                    currentIndex = indexOfValue(root.options.sortType)
                }
                onActivated:{
                    root.options.sortType = currentValue
                }
            }
        }
        StackView {
            id:stackView
            Layout.fillWidth: true
            Layout.fillHeight: true
            replaceEnter:Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 0
                    to:1
                    duration: 0
                }
            }

            replaceExit: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 1
                    to:0
                    duration: 0
                }
            }
            initialItem: Item{}
        }
    }
}
