import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    required property var title
    required property var image
    color:"#0078d4"
    radius:3
    height: titleLayout.implicitHeight
    width: titleLayout.implicitWidth + 10
    RowLayout {
        id:titleLayout
        Image {
            Layout.preferredHeight:38
            Layout.preferredWidth:38

            source: image
        }
        Label {
            text:title
            font.pixelSize: 20
            font.bold: true
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked:  {}
    }
}
