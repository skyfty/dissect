import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

import Halve 1.0
Control {
    id:root
    required property var profile
    required property var model
    property real range: model.max - model.min
    onRangeChanged: {
        readject();
    }
    Connections {
        target: model
        function onChanged() {readject();}
    }
    function readject() {
        var ratioInWidth= root.range / barContent.height;
        barItem.height = Math.min(model.height / ratioInWidth, barContent.height);
        barItem.y = getAbsValue(model.pos) / ratioInWidth;
        if (barItem.y < 0) {
            barItem.y = 0;
        } else if (barItem.y + barItem.height >= barContent.height){
            barItem.y = barContent.height - barItem.height;
        }
    }
    function getAbsValue(v) {
        if (v < 0) {
            v = model.max + Math.abs(v)
        } else {
            v = model.max - v;
        }
        return v;
    }

    function formatLabelText(v) {
        if (profile.mappingType === Halve.Lat) {
            return Math.floor(v / AppSettings.samplingRate * 1000) + " ms"
        } else {
             return v.toFixed(1) + " mV"
        }
    }

    onHeightChanged: {
        readject();
    }
    RowLayout {
        anchors.fill: parent
        spacing: 2
        ColumnLayout {
            Layout.fillHeight: true
            Layout.maximumWidth: 40
            spacing: 0
            Label {
                text:formatLabelText(model.max)
                Layout.minimumHeight: 20
                Layout.maximumHeight: 20
                Layout.fillWidth: true
                horizontalAlignment:Text.AlignRight
                Layout.topMargin: 10
                Menu {
                    id: maxEditDialog
                    contentItem:Rectangle {
                        clip: true
                        SpinBox {
                            anchors.fill: parent
                            id:maxInput
                            editable:true
                            width: 100
                            focus: true
                            from: 0
                            value:model.max
                            to:65535
                        }

                    }
                    onAboutToHide: {
                        model.max = maxInput.value
                        maxEditDialog.close()
                    }
                }
            }

            Item {
                id:labelItem
                Layout.fillWidth: true
                Layout.fillHeight: true
                Item {
                    id:topLabel
                    width: parent.width
                    y:barItem.y - 15

                    Label {
                        width: parent.width
                        height: parent.height
                        y:topLabel.y <0?Math.abs(topLabel.y):0
                        text: formatLabelText(model.top)
                        horizontalAlignment:Text.AlignRight
                    }
                }

                Item {
                    id:lableMiddle
                }
                Item {
                    id:bottomLabel
                    width: parent.width
                    property int bottomLabelPos:topLabel.y + barItem.height
                    y: bottomLabelPos- 10
                    property int bottomLabelPos2:(minLable.y - 10) -  (bottomLabel.y + bottomLabel.height)

                    Label {
                        id:aaa
                        width: parent.width
                        height: parent.height
                        text: formatLabelText(model.bottom)
                        y:bottomLabel.bottomLabelPos2 < 35?-10:0
                        horizontalAlignment:Text.AlignRight
                    }
                }
            }

            Label {
                id:minLable
                text:formatLabelText(model.min)
                Layout.minimumHeight: 20
                Layout.maximumHeight: 20
                Layout.fillWidth: true
                horizontalAlignment:Text.AlignRight
                Layout.bottomMargin: 10
                Menu {
                    id: minEditDialog
                    contentItem:Rectangle {
                        clip: true
                        SpinBox {
                            id:minInput
                            anchors.fill: parent
                            editable:true
                            width: 100
                            focus: true
                            from: 0
                            value:model.min
                            to:-65535
                        }

                    }
                    onAboutToHide: {
                        model.min = minInput.value
                        minEditDialog.close()
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
            ColumnLayout {
                anchors.fill: parent
                spacing: 1

                SpinButton {
                    id:upSpinButton
                    enabled: profile.mappingType === Halve.Voltage
                    opacity: 0.0
                    Layout.fillWidth: true
                    Layout.rightMargin: 2
                    Layout.leftMargin: 2
                    Layout.topMargin: 0
                    Layout.bottomMargin: 0
                    onIncrease: {
                        model.increaseMax();
                    }
                    onDecrease: {
                        model.decreaseMax();
                    }
                    states: State {
                        name: "active"
                        when: root.hovered && upSpinButton.enabled
                        PropertyChanges { target: upSpinButton; opacity: 1.0 }
                    }

                    transitions: Transition {
                        from: "active"
                        SequentialAnimation {
                            PauseAnimation { duration: 450 }
                            NumberAnimation { target: upSpinButton; duration: 200; property: "opacity"; to: 0.0 }
                        }
                    }
                }

                Item {
                    id:barContent
                    // clip: true
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Rectangle {
                        anchors.fill: parent
                        color:"lightgray"
                        border.color: "white"
                        border.width: 1
                    }
                    Item {
                        id:barItem
                        anchors.left: barContent.left
                        anchors.right: barContent.right
                        anchors.margins: 0
                        y:0
                        height: barContent.height
                        property var persistY;
                        Canvas {
                            id:canvas
                            anchors.fill: parent
                            onPaint: {
                                var ctx = getContext("2d");
                                var gradient = ctx.createLinearGradient(0,0,0, barItem.height)
                                var linearGradient = model.linearGradient;
                                for(var i = 0; i < linearGradient.length; ++i) {
                                    var scalar = linearGradient[i];
                                    gradient.addColorStop(scalar.begin, scalar.beginColor)
                                    gradient.addColorStop(scalar.end, scalar.endColor)
                                }
                                ctx.fillStyle = gradient;
                                ctx.fillRect(0, 0, barItem.width, barItem.height);
                            }
                        }
                        function changeAreaPos(mouseY) {
                            var tty = mouseY - barItem.persistY;
                            var ratioInWidth= root.range / barContent.height;
                            var ty = barItem.y + tty;
                            if (ty < 0) {
                                root.model.pos = model.max;
                                return;
                            }
                            if (ty + barItem.height >= barContent.height) {
                                if (root.model.pos < 0) {
                                    root.model.pos = model.min + Math.abs(root.model.height)
                                } else {
                                    root.model.pos = model.min + root.model.height;
                                }
                                return;
                            }
                            root.model.pos -= (tty * ratioInWidth);
                        }
                        Item {
                            anchors.fill: parent
                            MouseArea {
                                anchors.fill: parent
                                onPressed: {
                                    barItem.persistY = mouseY
                                }
                                cursorShape: Qt.SizeVerCursor
                                onPositionChanged: {
                                    barItem.changeAreaPos(mouseY)
                                }
                            }
                        }
                    }

                }
                SpinButton {
                    id:downSpinButton
                    enabled: profile.mappingType === Halve.Voltage
                    opacity: 0.0
                    Layout.fillWidth: true
                    onIncrease: {
                        model.increaseMin();
                    }
                    onDecrease: {
                        model.decreaseMin();
                    }

                    states: State {
                        name: "active"
                        when: root.hovered && downSpinButton.enabled
                        PropertyChanges { target: downSpinButton; opacity: 1.0 }
                    }

                    transitions: Transition {
                        from: "active"
                        SequentialAnimation {
                            PauseAnimation { duration: 450 }
                            NumberAnimation { target: downSpinButton; duration: 200; property: "opacity"; to: 0.0 }
                        }
                    }
                }
            }
        }

    }
}
