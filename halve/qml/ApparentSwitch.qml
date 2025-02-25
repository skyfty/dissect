
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

CheckBox {
    id:root
    indicator: Item {
         implicitWidth: 25
         implicitHeight: 25
         x: 0
         y: parent.height / 2 - height / 2
         opacity: enabled ? 1.0 : 0.2
         Image {
             anchors.fill: parent
             source: checked?"qrc:/assets/images/visible.svg":"qrc:/assets/images/invisible.svg"
         }
     }
    background: Rectangle {
        id: panel
        visible: root.down || (root.enabled && root.hovered)
        color: root.enabled && root.hovered?"#464646":"#3c3c3c"
        radius: 2
        Rectangle {
            x: 1; y: 1
            width: parent.width - 2
            height: parent.height - 2
            border.color: Fusion.innerContrastLine
            color: "transparent"
            radius: 2
        }
    }
}
