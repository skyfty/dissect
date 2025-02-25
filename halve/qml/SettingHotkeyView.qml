import QtQuick
import QtCore
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtQuick.Controls

Rectangle{
    anchors.fill: parent
    radius:10
    anchors.margins: 10
    color:"#3d3d3d"

    Item {
        anchors.fill: parent
        anchors.margins: 10
        GridLayout {
            columns: 2
            anchors.horizontalCenter: parent.horizontalCenter

        }
    }
}
