
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels 1.0
import QtQuick.Window

Pane {
    id:root
    required property var profile
    property alias model: currentCatheterTableModel.sourceModel
    padding: 1

    clip: true
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        RowLayout {
            id:toolBar
            Layout.fillWidth: true
            Layout.maximumHeight:  34
            Layout.minimumHeight: 34
            Layout.alignment:Qt.AlignVCenter

            SearchInput {
                id:searchInput
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.margins: 2
                Layout.maximumHeight: 30

                model: currentCatheterTableModel
            }
        }

        CatheterSortFilterProxyModel {
            id:currentCatheterTableModel
        }

        CatheterTypeModel {
            id:catheterTypeModel
            catheterTypeDb:ProfileManager.catheterRepertory.catheterTypeDb
        }
        CatheterTempletFilterModel {
            id:catheterTempletListModel
            sourceModel: CatheterTempletListModel {
                profile:root.profile
                catheterTempletDb:ProfileManager.catheterRepertory.catheterTempletDb
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                Layout.fillHeight: true
                Layout.maximumWidth: 27
                ToolButton {
                    icon {
                        source: "qrc:/assets/images/delete.png"
                        height: 19
                        width:19
                    }

                    onClicked:  {
                        deleteConfirmDialog.open();
                    }

                    ConfirmDialog {
                        id: deleteConfirmDialog
                        text: qsTr("Are you sure you want to delete this?")
                        prompt: qsTr("Delete catheter data at the configured catheter box and signal settings")
                        onAccepted: {
                            paramsPanel.pop();
                            currentCatheterTableModel.removeRows(catheterTable.currentRow, 1)
                        }
                    }
                    hoverEnabled: true
                    enabled: root.profile.type !== Profile.SNAPSHOT && catheterTable.currentRow !== -1
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Delete")
                }

                Component {
                    id:editDialogComponent
                    Menu {
                        id: editDialogPopup
                        contentItem:Pane {
                            focusPolicy: Qt.ClickFocus
                            TextForm {
                                property int index:-1
                                anchors.fill: parent
                                onAccepted: {
                                    var name = text.trim();
                                    if (name !== "") {
                                        if (catheterTempletListModel.indexOfName(name) !== -1) {
                                            return;
                                        } else {
                                            catheterTempletListModel.add(name);
                                        }
                                    }
                                    editDialogPopup.close();
                                }
                            }
                        }
                        onAboutToHide: {
                            editDialogPopup.destroy();
                        }
                    }
                }

                ToolButton {
                    icon {
                        source: "qrc:/assets/images/save.png"
                        height: 19
                        width:19
                    }

                    onClicked:  {
                        editDialogComponent.createObject(parent).open();
                    }

                    hoverEnabled: true
                    enabled: root.profile.type !== Profile.SNAPSHOT &&  catheterTable.count !== 0
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Save as templet")
                }

                Component {
                    id:catheterTempletViewComponent
                    Menu {
                        id: mappingSettingPopup
                        contentItem:Pane {
                            focusPolicy: Qt.ClickFocus
                            CatheterTempletView {
                                id:catheterTempletView
                                anchors.fill: parent
                                model: catheterTempletListModel
                                onClose: {
                                    mappingSettingPopup.close();
                                }

                                onAccepted: {
                                    catheterTempletListModel.import(catheterTempletView.currentRow)
                                    mappingSettingPopup.close();
                                }
                            }
                        }
                        onAboutToHide: {
                            mappingSettingPopup.destroy();
                        }
                        width: 300
                        height: 400
                    }
                }

                ToolButton {
                    icon {
                        source: "qrc:/assets/images/import.png"
                        height: 19
                        width:19
                    }

                    enabled:root.profile.type !== Profile.SNAPSHOT
                    onClicked:  {
                        catheterTempletViewComponent.createObject(parent).open();
                    }

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("import a templet")
                }

                Item {
                    Layout.fillHeight: true

                }
            }

            SplitView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                orientation: Qt.Vertical

                CatheterTable {
                    id:catheterTable
                    SplitView.fillWidth: true
                    SplitView.fillHeight: true
                    model:currentCatheterTableModel
                    catheterTypeModel:catheterTypeModel
                    revisable:root.profile.type !== Profile.SNAPSHOT
                    channelMode: root.profile.channelMode
                    onCurrentRowChanged: {
                        var currentCatheter = currentCatheterTableModel.getData(currentRow);
                        if (currentCatheter !== null) {
                            paramsPanel.replace("CatheterForm.qml",{catheter:currentCatheter});
                        }
                    }
                }

                StackView {
                    id:paramsPanel
                    visible: catheterTable.currentRow!== -1
                    SplitView.fillWidth: true
                    SplitView.preferredHeight: 230
                    replaceEnter:Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 0
                            to:1
                            duration: 200
                        }
                    }

                    replaceExit: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 1
                            to:0
                            duration: 200
                        }
                    }
                }
            }
        }


    }
}
