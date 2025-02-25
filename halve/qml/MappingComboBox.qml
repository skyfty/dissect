import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts

Rectangle {
    id:root
    property alias model:control.model
    property alias currentIndex:control.currentIndex
    property alias currentValue:control.currentValue
    property bool active: true

    function indexOfValue(v) {
        return control.indexOfValue(v);
    }

    signal activated(var currentValue);
    signal add();
    signal remove(var index);
    signal edit(var index, var text);

    color:"#282828"
    border.color: control.pressed ? "#0078d4" : "#212133"
    border.width: control.visualFocus ? 2 : 1
    radius: 1
    height: 30

    Component {
        id:deleteConfirmDialogComponent
        ConfirmDialog {
            id: deleteConfirmDialog
            required property var index
            text: qsTr("Are you sure you want to delete this?")
            onAccepted: {
                if (index < control.currentIndex) {
                    remove(index)
                    control.currentIndex--;
                } else {
                    remove(index)
                }
            }
        }
    }


    Component {
        id:editDialogComponent
        EditForm {
            id:editForm
            required property int index
            maximumLength: 20
            onAccepted: {
                var name = text.trim();
                if (name !== "") {
                    root.edit(index, name);
                }
                editForm.destroy()
            }
        }
    }

    RowLayout {
        spacing:0
        anchors.fill: parent
        anchors.margins: 1
        ComboBox {
            id:control
            Layout.fillWidth: true
            Layout.margins: 1
            clip: true

            focusPolicy:Qt.NoFocus
            background: Item {
                implicitWidth: 120
                implicitHeight: 24
            }


            contentItem:  Label {
                clip: true
                topPadding: 4
                leftPadding: 4 - control.padding
                rightPadding: 30
                bottomPadding: 4
                text: control.displayText
                font.pixelSize: 13
                color: "#ffffff"
                verticalAlignment: Text.AlignVCenter
            }
            delegate: MenuItem {
                id: menuItem
                width: ListView.view.width
                text: model[control.textRole]
                font.weight: control.currentIndex === index ? Font.Black : Font.Normal
                highlighted: control.highlightedIndex === index
                hoverEnabled: control.hoverEnabled

                contentItem: Item {
                    RowLayout {
                        spacing: 0
                        width: parent.width
                        anchors.verticalCenter: parent.verticalCenter
                        Label {
                            clip: true
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            id:iconLabel
                            verticalAlignment:Text.AlignVCenter
                            text: menuItem.text
                            font: menuItem.font
                            color: menuItem.down || menuItem.highlighted ? "white" : control.palette.text
                        }

                        Button {
                            icon {
                                height: 14
                                width: 14
                                source: "qrc:/assets/images/erase.png"
                            }
                            visible: control.currentValue !== id && menuItem.highlighted
                            enabled: control.currentValue !== id && root.active

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
                            Layout.alignment:Qt.AlignRight
                            background: Item{}
                            onClicked: {
                                editDialogComponent.createObject(parent, { index:index,text:menuItem.text }).open();
                            }
                            enabled: root.active

                        }


                    }
                }
            }
            textRole: "name"
            valueRole: "id"
            onActivated: root.activated(currentValue)
        }

        ToolButton {
            Layout.fillHeight: true
            Layout.preferredWidth: 20
            icon {
                source: "qrc:/assets/images/add2.svg"
                height: 10
                width:10
            }
            onClicked: add()
            enabled: root.active
            hoverEnabled: true
            ToolTip.visible: hovered
            ToolTip.text: qsTr("标测设置")
        }
    }

}

