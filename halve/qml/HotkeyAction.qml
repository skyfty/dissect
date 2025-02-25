import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve

Item {
    id:root
    required property var profile
    required property var name
    signal activated()

    Connections {
        enabled: root.enabled
        target: root.profile
        function onAction(name) {
            if (name === root.name) {
                root.activated()
            }
        }
    }
}
