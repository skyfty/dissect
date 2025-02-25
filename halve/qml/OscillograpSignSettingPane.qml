import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts

 ColumnLayout {
     id:root
     required property var profile
     required property var catheterListModel
     property var electrodeList : root.profile.oscillograpElectrode

    GroupBox {
        id:bodySurfaceGroupBox
        ButtonGroup {
            id:bodySurfaceGroupBoxGroup
            exclusive:false
        }
        label: RowLayout {
            width: parent.width
            Label {
                text:"ECG"
            }
            Item {
                Layout.fillWidth: true
            }

            CheckBox {
                text:"全选"
                checkState :bodySurfaceGroupBoxGroup.checkState
                onClicked: {
                    for(var i = 0; i < bodySurfaceElectrodeNodeListModel.rowCount(); ++i) {
                        var item = bodySurfaceElectrodeNodeListModel.get(i)
                        if (checked) {
                            electrodeList.add(item)
                        } else {
                            electrodeList.remove(item)
                        }
                    }
                    bodySurfaceElectrodeNodeListModel.reset();
                }
            }
        }

        Layout.fillWidth: true
        GridLayout {
            anchors.fill: parent
            columns: 6
            columnSpacing:9
            Repeater {
                model: BodySurfaceElectrodeNodeListModel{
                    id:bodySurfaceElectrodeNodeListModel
                    profile:root.profile
                }
                delegate: RowLayout {
                    required property int index
                    required property var name
                    required property var id
                    CheckBox {
                        text:qsTr(name)
                        ButtonGroup.group: bodySurfaceGroupBoxGroup
                        checked: electrodeList.exist(id)
                        onClicked: {
                            if (checked) {
                                electrodeList.add(id)
                            } else {
                                electrodeList.remove(id)
                            }
                        }
                    }
                }
            }
        }
    }


    GroupBox {
        id:pantGroupBox
        visible: root.profile.pantSampling
        ButtonGroup {
            id:pantGroupBoxGroup
            exclusive:false
        }
        label: RowLayout {
            width: parent.width
            Label {
                text:qsTr("Respiration")
            }
            Item {
                Layout.fillWidth: true
            }

            CheckBox {
                text:qsTr("All")
                checkState :pantGroupBoxGroup.checkState
                onClicked: {
                    for(var i = 0; i < pantElectrodeListNode.rowCount(); ++i) {
                        var item = pantElectrodeListNode.get(i)
                        if (checked) {
                            electrodeList.add(item)
                        } else {
                            electrodeList.remove(item)
                        }
                    }
                    pantElectrodeListNode.reset();
                }
            }
        }

        Layout.fillWidth: true
        GridLayout {
            anchors.fill: parent
            columns: 6
            columnSpacing:9
            Repeater {
                model: PantElectrodeListNode {
                    id:pantElectrodeListNode
                    profile:root.profile
                }
                delegate: RowLayout {
                    required property int index
                    required property var name
                    required property var id
                    CheckBox {
                        text:qsTr(name)
                        ButtonGroup.group: pantGroupBoxGroup
                        checked: electrodeList.exist(id)
                        onClicked: {
                            if (checked) {
                                electrodeList.add(id)
                            } else {
                                electrodeList.remove(id)
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id:oscillograpMoreSignSettingPaneComponent
        Item {
            id:layout
            required property var catheter
            required property var nodeListModel
            width: oscillograpMoreSignSettingPane.width
            height: oscillograpMoreSignSettingPane.height
            OscillograpMoreSignSettingPane {
                id:oscillograpMoreSignSettingPane
                profile:root.profile
                anchors.fill: parent
                catheter:layout.catheter
                effectiveElectrodeNodeListModel:layout.nodeListModel
            }
        }
    }

    Repeater {
        model: catheterListModel
        delegate: GroupBox {
            required property var name
            required property int index
            property var currentCatheter:catheterListModel.get(index)
            visible: currentCatheter !== null && currentCatheter.employ

            ButtonGroup {
                id:catheterElectrodeNodeGroupBoxGroup
                exclusive:false
            }
            OscillograpCatheterElectrodeNodeListModel{
                id:oscillograpNodeListModel
                profile:root.profile
                catheter:currentCatheter
            }
            label: RowLayout {
                width: parent.width
                Label {
                    text:currentCatheter !== null &&currentCatheter.type
                }
                Item {
                    Layout.fillWidth: true
                }

                CheckBox {
                    text:qsTr("All")
                    checkState :catheterElectrodeNodeGroupBoxGroup.checkState

                    onClicked: {
                        for(var i = 0; i < oscillograpNodeListModel.rowCount(); ++i) {
                            var item = oscillograpNodeListModel.get(i)
                            if (checked) {
                                electrodeList.add(item)
                            } else {
                                electrodeList.remove(item)
                            }
                        }
                        oscillograpNodeListModel.reset();
                    }
                }

                ToolButton {
                    Layout.alignment: Qt.AlignRight|Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/more.svg"
                        height: 19
                        width:19
                    }
                    onClicked: moreSettingPopup.open()
                    Menu {
                        id: moreSettingPopup
                        contentItem:Pane {
                            focusPolicy: Qt.ClickFocus
                            StackView {
                                anchors.fill: parent
                                id:oscillograpMoreSignSettingPaneView
                                Layout.fillHeight: true
                                Layout.fillWidth: true
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

                        onAboutToHide: {
                            oscillograpMoreSignSettingPaneView.pop();
                        }

                        onAboutToShow: {
                            oscillograpMoreSignSettingPaneView.replace(oscillograpMoreSignSettingPaneComponent,{catheter:currentCatheter, nodeListModel:oscillograpNodeListModel});
                            var h = oscillograpMoreSignSettingPaneView.currentItem.height;
                            var w = oscillograpMoreSignSettingPaneView.currentItem.width;

                            moreSettingPopup.width =w + 20;
                            moreSettingPopup.height =h + 20;

                        }
                    }
                }
            }
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent
                spacing:9
                GridLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    columns: 6
                    columnSpacing:9
                    Repeater {
                        model: oscillograpNodeListModel
                        delegate: RowLayout {
                            required property int index
                            required property var name
                            required property var id
                            CheckBox {
                                text:qsTr(name)
                                ButtonGroup.group: catheterElectrodeNodeGroupBoxGroup
                                checked: electrodeList.exist(id)
                                onClicked: {
                                    if (checked) {
                                        electrodeList.add(id)
                                    } else {
                                        electrodeList.remove(id)
                                    }
                                    oscillograpNodeListModel.reset();
                                }
                            }
                        }
                    }
                }
                Item {
                    Layout.fillWidth: true
                }

            }
        }
    }
 }

