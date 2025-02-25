import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    color: "red"

    property int startX: 0
    property int startY: 0

    MouseArea {
        id: dragArea
        anchors.fill: parent
        drag.target: parent

        onPressed: {
            startX = mouse.x
            startY = mouse.y
        }

        onPositionChanged: {
            var deltaX = mouse.x - startX
            var deltaY = mouse.y - startY
            if (Math.abs(deltaX) > Math.abs(deltaY)) {
                dragArea.drag.axis = Drag.XAxis
            } else {
                dragArea.drag.axis = Drag.NoAxis
            }
        }
    }
}
