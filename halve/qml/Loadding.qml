import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts

Item {
    id:root
    property alias to:progressBar.to
    property alias value:progressBar.value
    property alias text:lable.text

    Rectangle {
        anchors.fill: parent
        color:"#21233a"
        Image {
            source: "qrc:/assets/images/settings.png"
            width:700
            height: 700
            opacity:0.02
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }


        ColumnLayout {
            anchors.centerIn: parent
            ProgressBar {
                id:progressBar
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 400
                Layout.preferredHeight: 10
                from: 0
                to:componentAmount
                value: componentInitCount
            }
            Label {
                id:lable
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: 20
                font.weight: Font.Bold
            }
        }
    }
}
