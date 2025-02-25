import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

Rectangle {
    id:flashScreen
    anchors.fill: parent
    opacity: 0
    SequentialAnimation {
        running: true
        PropertyAnimation {
            target: flashScreen
            property: "opacity"
            to: 1
            duration: 100
        }
        PropertyAnimation {
            target: flashScreen
            property: "opacity"
            to: 0
            duration: 100
        }

        onFinished: {
            flashScreen.destroy()
        }
    }
}
