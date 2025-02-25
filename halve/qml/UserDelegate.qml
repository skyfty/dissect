import QtQuick
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import Halve

Rectangle {
    id: root
    width: ListView.view.width
    height: button.implicitHeight + 22
    color: tapHandler.pressed ? "#404243" : button.containsMouse ? "#404243" : "#2e2f30"
    signal clicked()

    Item {
        id: button
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right:parent.right
        implicitHeight:row.implicitHeight
        property alias containsMouse: hoverHandler.hovered

        TapHandler {
            id: tapHandler
            onTapped: root.clicked()
        }
        HoverHandler {
            id: hoverHandler
        }
        RowLayout {
            id: row
            anchors.fill: parent
            implicitHeight:col.implicitHeight

            Image {
                Layout.margins: 5
                Layout.preferredWidth:60
                Layout.preferredHeight: 60
                source: sex === User.Man? "qrc:/assets/images/man.png": "qrc:/assets/images/woman.png"
            }

            Column {
                id:col
                Layout.fillWidth: true
                Text {
                    id: nameLabel
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    text: name + " [" + idcard + "]"
                    font.pixelSize: 22
                    color:"white"
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
            }
        }
    }
}
