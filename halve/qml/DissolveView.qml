import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtQuick.Dialogs
import "colorpicker"

Pane {
    id:root
    required property var profile
    required property var breathSurvey
    property var dissolveOptions : root.profile.dissolveOptions
    property var currentDissolvePointId : root.profile.currentDissolvePointId
    property var currentReseau : root.profile.currentReseau

    required property var combined

    //property bool tadkState: true
    property bool tadkState: root.combined.state === ChannelReplica.State_Tracking && root.currentReseau.apparent && root.currentReseau.pointNumber > 0
    padding: 1
    onCurrentDissolvePointIdChanged: {
        var pointId = dissolveModel.idOfRow(dissolveTable.currentRow);
        if (pointId === currentDissolvePointId) {
            return;
        }
        if (currentDissolvePointId === -1) {
            dissolveTable.scrollTo(-1);
        } else {
            dissolveTable.scrollTo(dissolveModel.rowOfId(currentDissolvePointId));
        }
    }

    DissolveDataSource {
        id:dissolveDataSource
        profile:root.profile
        combined:root.combined
        breathSurvey:root.breathSurvey
    }


    HotkeyAction {
        enabled:tadkState
        profile:root.profile
        name: "strikDissolve"
        onActivated: {
            dissolveDataSource.strik();
        }
    }
    DissolveListModel {
        id:dissolveListModel
        profile:root.profile
    }

    DissolveSortFilterProxyModel {
        id:dissolveModel
        onlyValid:dissolveOptions.onlyValid
        profile:root.profile
        sourceModel: dissolveListModel
        onOnlyValidChanged: {
            dissolveModel.invalidate();
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        RowLayout {
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
                model: dissolveModel
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout {
                Layout.fillHeight: true
                Layout.maximumWidth: 27
                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    property bool checkState: true
                    icon {
                        source: "qrc:/assets/images/checkbox_group.svg"
                        height: 19
                        width:19
                    }
                    onClicked:  {
                        for(var i = 0; i < childButtonGroup.buttons.length; ++i) {
                            childButtonGroup.buttons[i].checked = checkState;
                        }
                        checkState =!checkState;
                    }
                    enabled:dissolveTable.count > 0
                }
                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/erase.png"
                        height: 19
                        width:19
                    }

                    onClicked:  {
                        deleteConfirmDialog.open();
                    }
                    enabled:childButtonGroup.checkState != Qt.Unchecked
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Delete")

                    ConfirmDialog {
                        id: deleteConfirmDialog
                        text: qsTr("Are you sure you want to delete this?")
                        onAccepted: {
                            dissolveModel.removeRows(childButtonGroup.getCheckedRowIds());
                        }
                    }
                }
                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/palette.svg"
                        height: 19
                        width:19
                    }

                    onClicked:  {
                        colorPickerPopup.open()
                    }
                    enabled:childButtonGroup.checkState != Qt.Unchecked
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Palette")

                    Menu {
                        id: colorPickerPopup
                        contentItem: Pane {
                            focusPolicy: Qt.ClickFocus
                            padding: 0
                            ColumnLayout {
                                anchors.fill: parent
                                ColorPicker {
                                    width: 280
                                    height: 200
                                    enableDetails:false
                                    enableAlphaChannel:false
                                    onColorChanged: (changedColor) => {
                                        dissolveModel.setData(childButtonGroup.getCheckedRowIds(), changedColor, DissolveListModel.DYESTUFF);
                                    }
                                }
                            }
                        }
                    }
                }

                ToolButton {
                    icon {
                        source: "qrc:/assets/images/mapping.png"
                        height: 19
                        width:19
                    }
                    onClicked: dissolveSettingPopup.open()
                    Menu {
                        id: dissolveSettingPopup
                        y: parent.height
                        contentItem:Pane {
                            focusPolicy: Qt.ClickFocus
                            DissolveSettingPane {
                                anchors.fill: parent
                                catheter:dissolveDataSource.currentCatheter
                                dissolveOptions: root.dissolveOptions
                                profile:root.profile
                            }
                        }
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("The calibration setting")
                }
                ToolSeparator{orientation:Qt.Horizontal;Layout.fillWidth: true}

                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/tadk.svg"
                        height: 19
                        width:19
                    }
                    enabled:tadkState
                    onClicked: {
                        dissolveDataSource.strik();
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Start")
                }
                Item {
                    Layout.fillHeight: true

                }
            }
            SplitView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                orientation: Qt.Vertical

                ButtonGroup {
                     id: childButtonGroup
                     exclusive: false
                     function getCheckedRowIds() {
                         var rowIds = [];
                         for(var i = 0; i < childButtonGroup.buttons.length; ++i) {
                             if (childButtonGroup.buttons[i].checked) {
                                 rowIds.push(childButtonGroup.buttons[i].rowId);
                             }
                         }
                         return rowIds;
                     }
                }
                DissolveTable {
                    id:dissolveTable
                    SplitView.fillWidth: true
                    SplitView.fillHeight: true
                    model:dissolveModel
                    buttonGroup:childButtonGroup
                    onCurrentRowChanged: {
                        if (dissolveTable.currentRow === -1) {
                            root.profile.currentDissolvePointId = dissolveTable.currentRow;
                        } else {
                            root.profile.currentDissolvePointId = dissolveModel.idOfRow(dissolveTable.currentRow);
                        }
                        if (dissolveTable.currentRow !== -1) {
                            paramsPanel.replace("DissolveForm.qml",{model:dissolveModel, row:dissolveTable.currentRow});
                        }
                    }
                }
                StackView {
                    id:paramsPanel
                    visible: dissolveTable.currentRow!== -1
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
