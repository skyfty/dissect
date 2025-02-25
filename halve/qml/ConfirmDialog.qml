
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels 1.0
import QtQuick.Window

Menu {
    id: root
    property alias text: contentLable.text
    property alias prompt: promptLable.text
    property bool delay: false

    signal accepted();
    signal rejected();

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

            Label {
                id:promptLable
                visible: promptLable.text.length > 0
                font.pixelSize: 12
                wrapMode:Text.Wrap
                Layout.bottomMargin: 7
                Layout.leftMargin: 3
                Layout.rightMargin: 3
            }
            RowLayout {
                Layout.fillWidth: true


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
        root.close()
    }
}
