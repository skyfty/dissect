
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
    property var options:profile.notebookOptions
    property var sortType:profile.notebookOptions.sortType

    onSortTypeChanged: {
        if (root.sortType !== Halve.NOTEBOOK_SORT_CATEGORY ) {
            notebookSortFilterModel.sort(0, Qt.AscendingOrder);
        }
    }

    function openCurrentNotebook() {
        var notebookId = notebookSortFilterModel.data(notebookSortFilterModel.index(notebookTable.currentRow,0), NotebookListModel.ID);
        root.source.open(notebookId);
        if (notebookModel.data(notebookTree.currentIndex, NotebookListModel.TYPE) === Halve.MAPPING) {
            root.profile.activeDockWidget("dockMappingView");
        }
    }

    NotebookListModel {
        id:notebookModel
        profile:root.profile
    }

    NotebookSortFilterModel {
        id:notebookSortFilterModel
        sourceModel: notebookModel
        sortRole: root.options.sortType
    }

    RowLayout {
        anchors.fill: parent
        ColumnLayout {
            id:toolBar
            Layout.fillHeight: true
            Layout.maximumWidth: 27
            function setEnabled() {
                var selected = notebookTable.currentRow !== -1;
                var type = notebookSortFilterModel.data(notebookSortFilterModel.index(notebookTable.currentRow,0), NotebookListModel.TYPE);
                deleteButton.enabled =selected &&
                        [Halve.AUTORECORD,
                         Halve.SNAPSHOT,
                         Halve.SCREENSHOT,
                         Halve.SCREENRECORD].indexOf(type) !== -1
                openButton.enabled =selected;
            }

            ToolButton {
                id:openButton
                icon {
                    source: "qrc:/assets/images/view.svg"
                    height: 19
                    width:19
                }
                onClicked: openCurrentNotebook()
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
                        notebookSortFilterModel.removeRows(notebookTable.currentRow, 1);
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
        NotebookTable {
            id:notebookTable
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: notebookSortFilterModel
            onCurrentRowChanged:toolBar.setEnabled()
            Component.onCompleted: toolBar.setEnabled()
            onDoubleTapped: openCurrentNotebook()
        }
    }

}
