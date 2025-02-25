
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels 1.0
import QtQuick.Window

Pane {
    id:root
    required property var profile
    property var reseauListModel
    required property var combined
    property var reproduceOptions: root.profile.reproduceOptions
    padding: 1
    clip: true

    RowLayout {
        anchors.fill: parent
        ColumnLayout {
            id:toolBar
            Layout.fillHeight: true
            Layout.maximumWidth: 27

            function setEnabled() {
                deleteButton.enabled = reseauTree.currentIndex.row !== -1 && !reseauListModel.data(reseauTree.currentIndex, ReseauListModel.INUSE)
                activeButton.enabled = reseauTree.currentIndex.row !== -1;;
            }

            ToolButton {
                icon {
                    source: "qrc:/assets/images/stage.png"
                    height: 19
                    width:19
                }
                onClicked: peproduceSettingPopup.open()
                Menu {
                    id: peproduceSettingPopup
                    y:parent.height
                    contentItem:Pane {
                        ReproduceSettingPane {
                            anchors.fill: parent
                            combined:root.combined
                            options: root.reproduceOptions
                        }
                    }
                    onAboutToHide: root.profile.save()
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Modeling parameters")
            }
            ToolButton {
                icon {
                    source: "qrc:/assets/images/add.png"
                    height: 19
                    width:19
                }
                action: Action {
                    onTriggered: reseauListModel.insertRows(0, 1);
                }

                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Add")
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
                        reseauListModel.removeRows(reseauTree.currentIndex.row, 1);
                    }
                }
            }

            ToolSeparator{orientation:Qt.Horizontal;Layout.fillWidth: true}

            ToolButton {
                id:activeButton
                icon {
                    source: "qrc:/assets/images/active.svg"
                    height: 19
                    width:19
                }
                action: Action {
                    onTriggered: {
                        reseauListModel.setData(reseauTree.currentIndex, true, ReseauListModel.INUSE);
                    }
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Active")
            }
            Item {
                Layout.fillHeight: true

            }
            Connections {
                target: root.profile
                function onCurrentReseauChanged(){
                    toolBar.setEnabled()
                }
                function onMappingReseauChanged() {
                    toolBar.setEnabled()
                }
            }

            Component.onCompleted: toolBar.setEnabled()
        }
        ReseauTable {
            id:reseauTree
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: reseauListModel
            reseauListModel:root.reseauListModel
            onCurrentIndexChanged:toolBar.setEnabled()
        }
    }
}
