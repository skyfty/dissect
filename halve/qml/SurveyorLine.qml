import QtQuick

import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtQuick.Controls
import "utils.js" as Utils

Rectangle {
    id:root
    required property var chartHeight
    height: Math.round(AppSettings.basisUnit * (Screen.pixelDensity / 25.4) * 25.4)
    width: 100
    color: "transparent"
    property int idx:4
    property var regulateList: Utils.SurveyorRegulateList
    property var value:regulateList[idx]

    function reset() {
        root.idx = root.regulateList.indexOf(value);
        root.height  = millimeterToPixel(root.idx);
    }

    onValueChanged: {
        reset()
    }
    Component.onCompleted: reset()

    Rectangle {
        id:biaogan
        width: 10
        height: parent.height
        color: "lightyellow"
        opacity: 0.7
    }

    Label {
        anchors.bottom: parent.bottom
        anchors.left: biaogan.right
        anchors.leftMargin: 3
        font.pixelSize: 13
        text:value + "mm/mV"
        color:"lightyellow"
        opacity: 0.9
    }

    function millimeterToPixel(idx) {
        return Math.round(value * (Screen.pixelDensity / 25.4) * 25.4)
    }

    MouseArea {
        anchors.fill: parent
        onWheel: {
            if (wheel.angleDelta.y > 0) {
                ++idx;
                if (idx === regulateList.length) {
                    idx = regulateList.length - 1
                }
                root.value = regulateList[idx];
            } else if (wheel.angleDelta.y < 0) {
                --idx;
                if (idx <= 0) {
                    idx = 0
                }
                root.value = regulateList[idx];
            }
        }
    }
}
