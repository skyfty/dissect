import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve

Row {
    id:root
    property alias model:proxyModel.sourceModel
    property bool active: true
    signal changed(var position);
    signal added(var name);
    signal rested();
    property alias resetEnabled:resetApButton.enabled

    Repeater {
        model: AzimuthProxyModel {
            id:proxyModel
        }

        ToolButton {
            required property int index
            required property var name
            required property var position

            Layout.alignment: Qt.AlignRight
            text: qsTr(name)
            onClicked:{
                changed(name);
            }
            hoverEnabled: true
            ToolTip.visible: hovered
            ToolTip.text: qsTr("Swap to ") + name
            font.bold: true
            font.pixelSize: 15
        }
    }
    Component {
        id:deleteConfirmDialogComponent
        ConfirmDialog {
            id: deleteConfirmDialog
            required property var index
            text: qsTr("Are you sure you want to delete this?")
            onAccepted: {
                root.model.removeRows(index, 1)
            }
        }
    }
    Component {
        id:editDialogComponent
        EditForm {
            id:editForm
            property int index:-1
            maximumLength: 20
            placeholderText: qsTr("Input name")
            onAccepted: {
                var name = text.trim();
                if (name !== "" && proxyModel.getData(name) === null) {
                    if (index !== -1) {
                        root.model.setData(root.model.index(index, 0), name, AzimuthModel.NAME);
                    } else {
                        root.added(name);
                    }
                }
                editForm.destroy()
            }

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
            y: parent.height
            contentItem:Pane {
                focusPolicy: Qt.ClickFocus
                ColumnLayout {
                    Repeater {
                        model: root.model
                        RowLayout {
                            required property int index
                            required property var name
                            required property bool enabled
                            required property bool buildin
                            onEnabledChanged: {
                                checkBox.checked = enabled
                            }
                            spacing: 0
                            CheckBox {
                                id:checkBox
                                text:qsTr(name)
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                onClicked: {
                                    root.model.setData(root.model.index(index, 0), checked, AzimuthModel.ENABLED);
                                }
                            }
                            Button {
                                icon {
                                    height: 14
                                    width: 14
                                    source: "qrc:/assets/images/erase.png"
                                }
                                visible: !buildin && root.active
                                Layout.alignment:Qt.AlignRight
                                background: Item{}
                                onClicked: {
                                    deleteConfirmDialogComponent.createObject(parent, { index:index }).open();
                                }
                            }
                            Button {
                                icon {
                                    height: 14
                                    width: 14
                                    source: "qrc:/assets/images/edit.png"
                                }
                                visible: !buildin && root.active
                                Layout.alignment:Qt.AlignRight
                                background: Item{}
                                onClicked: {
                                    editDialogComponent.createObject(parent, {width:parent.width, index:index,text:name }).open();
                                }
                            }
                        }
                    }

                    Button {
                        id:addButton
                        text:qsTr("Save current position")
                        Layout.minimumWidth: 120
                        Layout.fillWidth: true
                        enabled:root.active
                        onClicked: {
                            editDialogComponent.createObject(addButton, {width:addButton.width}).open();
                        }
                    }
                    Button {
                        id:resetApButton
                        text:qsTr("Reset AP position")
                        Layout.minimumWidth: 120
                        Layout.fillWidth: true
                        enabled:root.active
                        onClicked: {
                            root.rested();
                        }
                    }
                }

            }
        }
        hoverEnabled: true
    }
}
