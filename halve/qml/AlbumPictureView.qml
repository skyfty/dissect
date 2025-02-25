import QtQuick

import QtQuick.Controls
import QtQuick.Layouts
import Halve
import QtMultimedia


Item {
    id:root
    property alias source: image.source
    ScrollView {
        anchors.fill: parent
        Image {
            id:image
            anchors.centerIn: parent
        }
    }
}
