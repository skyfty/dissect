
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels 1.0
import QtQuick.Window

Item {
    id:root
    required property var profile
    required property var source

    NotebookTreeModel {
        id:notebookModel
        profile:root.profile
    }
    function openCurrentNotebook() {
        var notebookId = notebookModel.data(notebookTree.currentIndex, NotebookTreeModel.ID);
        root.source.open(notebookId);
        if (notebookModel.data(notebookTree.currentIndex, NotebookTreeModel.TYPE) === Halve.MAPPING) {
            root.profile.activeDockWidget("dockMappingView");
        }
    }
    RowLayout {
        anchors.fill: parent
        ColumnLayout {
            id:toolBar
            Layout.fillHeight: true
            Layout.maximumWidth: 27
            function setEnabled() {
                var selected = notebookTree.currentIndex.row !== -1;
                var type = notebookModel.data(notebookTree.currentIndex, NotebookTreeModel.TYPE);
                deleteButton.enabled =selected &&
                        [Halve.AUTORECORD,
                         Halve.SCREENSHOT,
                         Halve.SNAPSHOT,
                         Halve.SCREENRECORD].indexOf(type) !== -1
                openButton.enabled =selected && type !== Halve.GENRE;
            }
            ToolButton {
                id:openButton
                icon {
                    source: "qrc:/assets/images/view.svg"
                    height: 19
                    width:19
                }
                onClicked:  openCurrentNotebook()
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Open")

            }
            ToolButton {
                id:deleteButton
                icon {
                    source: "qrc:/assets/images/delete.png"
                    height: 19
                    width:19
                }
                onClicked:  {
                    deleteConfirmDialog.open();
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Delete")

                ConfirmDialog {
                    id: deleteConfirmDialog
                    text: qsTr("Are you sure you want to delete this?")
                    onAccepted: {
                        notebookModel.removeRows(notebookTree.currentIndex.row, notebookTree.currentIndex.column, notebookModel.parent(notebookTree.currentIndex));
                    }
                }
            }

            ToolSeparator{orientation:Qt.Horizontal;Layout.fillWidth: true}

            ToolButton {
                id:reviewButton
                icon {
                    source: "qrc:/assets/images/snapping.svg"
                    height: 19
                    width:19
                }
                onClicked:  {
                    if(root.source.snapping) {
                        root.source.snapshot(new Date().getTime());
                    } else {
                        root.source.beginSnapshot(new Date().getTime());
                    }
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Snapshot")

            }
            Item {
                Layout.fillHeight: true

            }
        }
        NotebookTree {
            id:notebookTree
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: notebookModel
            onCurrentIndexChanged:toolBar.setEnabled()
            onDoubleTapped: openCurrentNotebook()
        }
    }

}
