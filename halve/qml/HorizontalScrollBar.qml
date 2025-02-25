import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

Slider {
     id: root
     hoverEnabled: true
     orientation: Qt.Horizontal
     stepSize:1
     opacity: 0.0
     from: 0
     value: 1
     to: 0
     signal actived();

     onOpacityChanged: {
         if (opacity == 1.0) {
             actived();
         }
     }

     handle: Rectangle {
         x: root.leftPadding + root.visualPosition * (root.availableWidth - width)
         y: root.topPadding + ((root.availableHeight - height) / 2)
         implicitWidth: 30
         implicitHeight: 13
         radius: 3
         color: root.pressed ? "#2c5d87" : "#282828"
         border.width: root.visualFocus ? 2 : 1
         border.color: root.visualFocus ? root.palette.highlight : root.enabled ? root.palette.mid : root.palette.midlight
     }
     background: Rectangle {
         x: root.leftPadding
         y: root.topPadding + (root.availableHeight - height) / 2
         implicitWidth: 200
         width: root.availableWidth
         height: 13
         radius: 3
         color: root.palette.midlight
         scale: root.mirrored ? -1 : 1
     }
     states: State {
         name: "active"
         when: root.hovered
         PropertyChanges { target: root; opacity: 1.0 }
     }

     transitions: Transition {
         from: "active"
         SequentialAnimation {
             PauseAnimation { duration: 450 }
             NumberAnimation { target: root; duration: 200; property: "opacity"; to: 0.0 }
         }
     }
 }
