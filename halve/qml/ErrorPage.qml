import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 1000
    height: 800
    id: root
    title: qsTr("Halve")
    visibility:InterfaceSettings.fullScreen?Window.FullScreen:Window.Windowed

    Settings {
        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height
    }
    Rectangle {
        anchors.fill: parent
        color:"#f73636"
        Image {
            source: "qrc:/assets/images/error.png"
            width:700
            height: 700
            opacity:0.1
            anchors.centerIn: parent

        }
        Label {
            id:label
            font.pixelSize: 30
            font.weight: Font.Bold
            text:"加载病人信息失败"
            anchors.centerIn: parent
        }
    }
    Component.onCompleted: StartoverState.set()
}
