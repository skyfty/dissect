import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts

Item {
    id: root
    clip: true
    required property var max
    required property real gurgitation
    required property real breatheLimit
    required property real topIndex
    required property real bottomIndex
    required property real baseIndex


    property int lastHeight: 0

    signal accepted(var r);

    function isOverflow() {
        return handleLine.y < topArea.y || handleLine.y > bottomArea.y ;
    }

    onBreatheLimitChanged: {
        resetAreaLimtPosition();
    }

    onBottomIndexChanged: {
        resetAreaPosition();
    }

    onTopIndexChanged: {
        resetAreaPosition();
    }

    onGurgitationChanged: {
        var yCenter = root.height / 2;
        var ratioInWidth= root.max / root.height;
        var breatheCenter = root.breatheLimit / 2 / ratioInWidth;
        var b = (root.gurgitation / ratioInWidth * yCenter / breatheCenter) / 3
        var ypos = yCenter  - b  - handleLine.height / 2;
        if (ypos >= 0 && ypos < root.height) {
            handleLine.y = ypos;
        } else {
            handleLine.y = yCenter - handleLine.height / 2;
        }
    }

    function applyHandleLineColor() {
        handleLine.color =isOverflow()? "red":"lightgreen";
    }

    function applyPosition() {
        var yCenter = root.height / 2;
        var ratioInWidth= root.max / root.height;
        var yRatio =(topArea.y - topAreaLimit.y) / (topAreaLimit.y - yCenter);
        accepted(yRatio);
    }

    onHeightChanged: {
        if (root.lastHeight !== 0) {
            var ss = root.height / root.lastHeight ;
            topArea.y *= ss;
            bottomArea.y *= ss;
            topAreaLimit.y *= ss;
            bottomAreaLimit.y *= ss;
            handleLine.y *= ss;
        }
        root.lastHeight = root.height;
    }

    onVisibleChanged: {
        if (visible) {
            delayResetTimer.start();
        }
    }

    function resetAreaPosition() {
        var ratioInWidth= root.max / root.height;
        var yCenter = root.height / 2;
        var breatheCenter = root.breatheLimit / 2 / ratioInWidth;
        var b = (((root.topIndex - root.baseIndex) / ratioInWidth * yCenter / breatheCenter) - yCenter) / 3
        topArea.y = topAreaLimit.y - b;
        bottomArea.y = bottomAreaLimit.y + b;
    }

    function resetAreaLimtPosition() {
        var yCenter = root.height / 2;
        var ratioInWidth= root.max / root.height;
        var breatheCenter = root.breatheLimit / 2 / ratioInWidth;
        var b = breatheCenter/ 3 * yCenter / breatheCenter;
        topAreaLimit.y = (yCenter - b);
        bottomAreaLimit.y = (yCenter + b) ;

    }
    Timer {
        id:delayResetTimer
        interval: 300
        onTriggered: {
            var yCenter = root.height / 2;
            var ratioInWidth= root.max / root.height;
            handleLine.y =yCenter - handleLine.height / 2;
            resetAreaLimtPosition();
            resetAreaPosition();
        }
    }

    Rectangle {
        id:topAreaLimit
        width:parent.width
        height: 1
        color: "orange"
    }

    Rectangle {
        id:bottomAreaLimit
        width:parent.width
        height: 1
        color: "orange"
    }

    Rectangle {
        id:handleLine
        width:parent.width
        height: 3
        color: "lightgreen"
        opacity: 0.6
        onYChanged:{
            applyHandleLineColor();
        }
    }
    property var persistY;

    function changeAreaPos(mouseY, di) {
        var ratioInWidth= root.max / root.height;
        var yCenter = root.height / 2;
        var ty = topArea.y + ((mouseY - root.persistY) * di);
        var by = bottomArea.y - ((mouseY - root.persistY) * di);

        var breatheCenter = root.breatheLimit / 2 / ratioInWidth;
        var b = breatheCenter/ 3 * yCenter / breatheCenter;
        if (ty <= topAreaLimit.y &&ty>= (topAreaLimit.y - b)) {
            topArea.y = ty;
            bottomArea.y = by;
        }
    }
    Item {
        id:topArea
        width:parent.width
        height: 5
        Rectangle {
            width:parent.width
            height: 2
            anchors.verticalCenter: parent.verticalCenter
            color: "blueviolet"
        }
        Item {
            id:topAreaHandle
            anchors.fill: parent
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    root.persistY = mouseY
                }
                cursorShape: Qt.SizeVerCursor
                onPositionChanged: {
                    changeAreaPos(mouseY, 1);
                    applyPosition();
                }
            }
        }


        onYChanged:{
            applyHandleLineColor();
        }
    }

    Item {
        id:bottomArea
        width:parent.width
        height: 5

        Rectangle {
            width:parent.width
            height: 2
            anchors.verticalCenter: parent.verticalCenter
            color: "blueviolet"
        }

        Item {
            id:bottomAreaHandle
            anchors.fill: parent
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    root.persistY = mouseY
                }
                cursorShape: Qt.SizeVerCursor
                onPositionChanged: {
                    changeAreaPos(mouseY, -1)
                    applyPosition();
                }
            }
        }

        onYChanged:{
            applyHandleLineColor();
        }
    }
}
