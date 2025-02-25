import QtQuick 
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts

Item {
    id: root
    required property var setting
    property bool completed:false;
    property int lastWidth: 0;
    property alias leftAreaWidth:leftArea.width
    property alias rightAreaWidth:rightArea.width
    readonly property int spaceWidth:10
    signal accepted();

    function applyPosition() {
        var ratioInWidth= root.setting.max / root.width;
        setting.leftIndex =Math.round(leftArea.width * ratioInWidth);
        setting.rightIndex = Math.round(rightArea.x * ratioInWidth);
        setting.baseIndex = Math.round((dragHandle.x +(dragHandle.width/2)) * ratioInWidth);
        accepted();
    }

    function changePosition() {
        var ratioInWidth= root.setting.max / root.width;
        var li =Math.round(leftArea.width * ratioInWidth);
        var ri = Math.round(rightArea.x * ratioInWidth);
        var ci = Math.round((dragHandle.x +(dragHandle.width/2)) * ratioInWidth);
        setting.leftShifting =li - ci;
        setting.rightShifting = Math.abs(ci - ri);
    }

    onSettingChanged: {
        if (root.completed) {
            reset();
        }
    }

    function reset() {
        var ratioInWidth= root.setting.max / root.width;
        leftArea.width = root.setting.leftIndex / ratioInWidth;
        rightArea.x = root.setting.rightIndex / ratioInWidth;
        rightArea.width = root.width - rightArea.x
        dragHandle.x = root.setting.baseIndex / ratioInWidth - (dragHandle.width/2);
    }
    onWidthChanged: {
        if (root.completed) {
            rightArea.width = root.width - rightArea.x
        }
    }

    Component.onCompleted: {
        reset();
        root.completed = true;
    }

    Rectangle {
        height: 1
        color:"gray"
        opacity: 0.6
        anchors.left: leftArea.right
        anchors.right:rightArea.left
        anchors.bottom: parent.bottom
        Label {
            text:formatShifting(Math.abs(setting.leftShifting) + Math.abs(setting.rightShifting))
            anchors.bottom: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Item {
        id: dragHandle
        width: 5
        height: parent.height
        Rectangle {
            id:dragHandleLine
            width: 1
            height: parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            color: "orange"
        }
        MouseArea {
            anchors.fill: parent
            property var persistWidth;
            enabled:  !root.setting.striked
            onPressed: {
                persistWidth = mouseX
            }
            onReleased: {
                applyPosition()
            }

            cursorShape: Qt.SizeHorCursor
            onPositionChanged: {
                var rx = dragHandle.x + mouseX - persistWidth;
                if (rx < leftArea.width + root.spaceWidth) {
                    dragHandle.x = leftArea.width + root.spaceWidth;
                } else {
                    if (rx + dragHandle.width + root.spaceWidth > rightArea.x) {
                        dragHandle.x = rightArea.x - dragHandle.width -  root.spaceWidth;
                    } else {
                        dragHandle.x = rx;
                    }
                }
                changePosition();
            }
        }
    }


    Rectangle {
        id:leftArea
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        gradient: Gradient {
            orientation:Gradient.Horizontal
            GradientStop {
                position: 0.0
                color: "#17FFFFFF"
            }
            GradientStop {
                position: 0.9
                color: "#17FFFFFF"
            }
            GradientStop {
                position: 1.0
                color: "#2EFFFFFF"
            }
        }

        Item {
            id:leftAreaHandle
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width:40
            MouseArea {
                anchors.fill: parent
                property var persistWidth;
                enabled: !root.setting.striked
                onPressed: {
                    persistWidth = mouseX
                }
                onReleased:applyPosition()
                cursorShape: Qt.SizeHorCursor
                onPositionChanged: {
                    var lwidth = leftArea.width + mouseX - persistWidth;
                    if (lwidth < root.spaceWidth) {
                        leftArea.width = root.spaceWidth;
                    } else if (lwidth > dragHandle.x - root.spaceWidth) {
                        leftArea.width = dragHandle.x - root.spaceWidth;
                    } else {
                        leftArea.width = lwidth;

                    }
                    changePosition();
                }
            }
        }

        Label {
            text:formatShifting(setting.leftShifting)
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 30
            anchors.rightMargin: 5
        }

        Label {
            text:formatShifting(setting.leftShifting)
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.rightMargin: 5
        }
    }

    Rectangle {
        id:rightArea
        height:parent.height

        gradient: Gradient {
            orientation:Gradient.Horizontal

            GradientStop {
                position: 0.0
                color: "#2EFFFFFF"
            }

            GradientStop {
                position: 0.1
                color: "#17FFFFFF"
            }
            GradientStop {
                position: 0.9
                color: "#17FFFFFF"

            }
        }

        Item {
            id:rightAreaHandle
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width:40
            MouseArea {
                anchors.fill: parent
                property var persistWidth;
                enabled: !root.setting.striked
                onPressed: {
                    persistWidth = mouseX
                }
                cursorShape: Qt.SizeHorCursor

                onReleased:applyPosition()
                onPositionChanged: {
                    var rx = rightArea.x + mouseX - persistWidth;
                    if (rx < dragHandle.x + dragHandle.width + root.spaceWidth) {
                        rightArea.x = dragHandle.x + dragHandle.width + root.spaceWidth;
                    } else if (rx > (root.width -root.spaceWidth)) {
                        rightArea.x = root.width - root.spaceWidth;
                    } else {
                        rightArea.x = rx;
                        rightArea.width = root.width - rightArea.x
                    }
                    changePosition();
                }
            }
        }


        Label {
            text:formatShifting(setting.rightShifting)
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.topMargin: 30
            anchors.leftMargin: 5
        }

        Label {
            text:formatShifting(setting.rightShifting)
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.leftMargin: 5
        }
    }

    function formatShifting(v) {
        return Math.floor(v / AppSettings.samplingRate * 1000) + " ms";
    }
}
