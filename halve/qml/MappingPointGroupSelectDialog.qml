
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import QtQuick.Window

Menu {
    id: root
    required property var model
    property var currentValue
    property alias currentRow:control.currentIndex
    property alias text: contentLable.text
    signal accepted();
    signal rejected();
    signal add();

    function indexOfValue(v) {
        return control.indexOfValue(v);
    }

    background: Rectangle {
        color: "#191919"
        border.color: "#282828"
        border.width: 1
    }
    contentItem:Pane {
        focusPolicy: Qt.ClickFocus

        ColumnLayout {
            anchors.fill: parent

            Label {
                id:contentLable
                wrapMode:Text.Wrap
                font.bold: true
                font.pixelSize: 14
                Layout.topMargin: 7
                Layout.bottomMargin: 7
                Layout.leftMargin: 3
                Layout.rightMargin: 3
                Layout.minimumWidth: 250
            }
            ComboBox {
                id:control
                Layout.fillWidth: true
                Layout.margins: 1
                clip: true
                onCurrentIndexChanged: {
                    if (control.currentIndex!== -1) {
                        root.currentValue = root.model.data(root.model.index(control.currentIndex, 0), MappingPointGroupListModel.Id);
                    } else {
                        root.currentValue = "";
                    }
                }
                displayText:control.currentIndex!== -1?root.model.data(root.model.index(control.currentIndex, 0), MappingPointGroupListModel.Name):controlMenu.itemAt(0).text
                popup: Menu {
                    id:controlMenu
                    width: control.width
                    topMargin: 6
                    bottomMargin: 6
                    padding: 1
                    height: Math.min(contentItem.implicitHeight + 2, control.Window.height - topMargin - bottomMargin)
                    MenuItem {
                        text:qsTr("Please choose")
                        font.bold: control.currentIndex = -1
                        onTriggered: {
                            control.currentIndex = -1;
                        }
                        }
                    Repeater {
                        model:root.model
                        MenuItem {
                            required property int index
                            required property string name
                            required property string id
                            text:name
                            font.bold: control.currentIndex === index
                            onTriggered: {
                                control.currentIndex = index
                            }
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true

                Button {
                    id:newButton
                    Layout.fillWidth: true
                    text: qsTr("New")
                    implicitHeight: 20
                    onClicked: {
                        root.add();
                    }
                    background: Rectangle {
                        implicitWidth: 80
                        implicitHeight: 24
                        radius: 2
                        color: newButton.down ? "#31609b": newButton.hovered?"#6493ce":"#404040";
                    }
                }

                Item {
                    Layout.fillWidth: true

                }

                Button {
                    id:okButton
                    Layout.fillWidth: true
                    text: qsTr("Yes")
                    implicitHeight: 20
                    onClicked: {
                        root.accepted();
                        root.close();
                    }
                    background: Rectangle {
                        implicitWidth: 80
                        implicitHeight: 24
                        radius: 2
                        color: okButton.down ? "#31609b": "#6493ce";
                    }
                }

                Button {
                    id:noButton
                    Layout.fillWidth: true
                    text: qsTr("No")
                    implicitHeight: 20
                    onClicked: {
                        root.rejected();
                        root.close()
                    }
                    background: Rectangle {
                        implicitWidth: 80
                        implicitHeight: 24
                        radius: 2
                        color: noButton.down ? "#31609b": noButton.hovered?"#6493ce":"#404040";
                    }
                }
            }
        }
    }
    onAboutToHide: {
        root.destroy();
    }
}
