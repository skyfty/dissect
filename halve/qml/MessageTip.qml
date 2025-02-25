
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels



Menu {
    id: root
    property alias text: contentLable.text

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
                font.bold: true
                font.pixelSize: 14
                Layout.topMargin: 7
                Layout.bottomMargin: 7
                Layout.leftMargin: 3
                Layout.rightMargin: 3
            }

            Button {
                id:okButton
                Layout.fillWidth: true
                text: qsTr("Ok")
                implicitHeight: 20
                onClicked: {
                    root.close()
                }
                background: Rectangle {
                    implicitWidth: 80
                    implicitHeight: 24
                    radius: 2
                    color: okButton.down ? "#31609b": "#6493ce";
                }
            }

        }
    }
}
