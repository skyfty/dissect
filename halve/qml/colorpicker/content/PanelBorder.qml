//  Fancy pseudo-3d control border
import QtQuick

Rectangle {
    width : 40; height : 15; radius: 2
    border.width: 1; border.color: _activePalette.light
    color: "transparent"
    anchors.leftMargin: 1; anchors.topMargin: 3
    clip: true
    Rectangle {
        anchors.fill: parent; radius: 2
        anchors.leftMargin: -1; anchors.topMargin: -1
        anchors.rightMargin: 0; anchors.bottomMargin: 0
        border.width: 1; border.color: _activePalette.mid
        color: "transparent"
    }
}
