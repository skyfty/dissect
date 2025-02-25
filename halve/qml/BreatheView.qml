import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve

Item {
    id:root
    required property var profile
    property var breathOptions:profile.breathOptions
    required property var breathSurvey
    clip: true


    ColumnLayout {
        anchors.fill: parent
        Label {
            visible: breathSurvey.abnormal
            Layout.fillWidth: true
            color: "white"
            wrapMode:Text.WordWrap
            padding: 5
            text:qsTr("Abnormal breathing detected; the collected breathing data is unavailable. Please re-collect the breathing data when in a stable breathing state.")

            background: Rectangle {
                color: "red"
                border.color: "#282828"
                border.width: 1
                radius:3
            }
        }

        SplitView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 4
            handle: Rectangle {
                implicitWidth: 4
                implicitHeight: parent.height
                color:"transparent"
            }
            ColumnLayout {
                SplitView.fillHeight: true
                SplitView.preferredWidth: 200
                Label {
                    text:qsTr("Respiration(%)")
                    Layout.fillWidth: true
                    color:"white"
                    font.pixelSize: 18
                    font.bold: true
                    horizontalAlignment:Text.AlignHCenter
                }
                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.topMargin: 5

                    RowLayout {
                        anchors.fill: parent
                        spacing: 0
                        Scaleplate {
                            id:scaleplate
                            Layout.fillHeight: true
                            Layout.preferredWidth: 15
                            showLable:false
                            maxValue:AppSettings.breathScaleplateMax
                            minValue: -AppSettings.breathScaleplateMax
                            step:100
                            minorTicks:4
                            vertical:true
                        }

                        Item {
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            clip: true

                            Canvas {
                                id:canvasScaleplateDegree
                                anchors.fill: parent
                                onPaint: {
                                    var ctx = getContext("2d");
                                    ctx.fillStyle = "white";
                                    ctx.font = "13px sans-serif"; // 设置数字字体大小和样式
                                    ctx.textAlign = "left";
                                    for (var i = scaleplate.minValue; i <= scaleplate.maxValue; i += scaleplate.step) {
                                        var y = (i - scaleplate.minValue) / (scaleplate.maxValue - scaleplate.minValue) * canvasScaleplateDegree.height;
                                        var txt = (i * -1);
                                        if (i === scaleplate.minValue) {
                                            y += 10;
                                        } else if (i === scaleplate.maxValue){
                                            y -= 1;
                                        } else {
                                            y += 6 - ((i - scaleplate.minValue) / scaleplate.step);
                                        }

                                        var x = canvasScaleplateDegree.width / 2 - 5;
                                        if (txt.length === 3) {
                                            x -= 8;
                                        } else if (txt.length === 4) {
                                            x -= 10;
                                        } else if (txt.length === 5) {
                                            x -= 12;
                                        }

                                        ctx.fillText(txt, x, y);
                                    }
                                }
                            }

                        }

                        Scaleplate {
                            Layout.fillHeight: true
                            Layout.preferredWidth: 15
                            showLable:scaleplate.showLable
                            maxValue:scaleplate.maxValue
                            minValue: scaleplate.minValue
                            step:scaleplate.step
                            minorTicks:scaleplate.minorTicks
                            vertical:true
                            mirror:true
                        }
                    }

                    TideWindow {
                        anchors.fill: parent
                        max:root.breathOptions.breatheMax * 2
                        gurgitation:root.breathSurvey.gurgitation
                        breatheLimit:root.breathOptions.breatheLimit
                        baseIndex:root.breathOptions.breatheBase
                        topIndex:root.breathOptions.breatheLimitTop
                        bottomIndex:root.breathOptions.breatheLimitBottom
                        onAccepted:(r)=> {
                            root.breathOptions.breathBoundaryRatio = Math.floor(r* 100);
                        }
                        Layout.topMargin: 5
                        Layout.bottomMargin: 5
                    }
                }

            }

            ColumnLayout {
                SplitView.fillHeight: true
                SplitView.fillWidth: true
                visible: root.breathOptions.breatheCompensation
                Label {
                    text:qsTr("Breathe Compensation")
                    Layout.fillWidth: true
                    color:"white"
                    font.pixelSize: 18
                    font.bold: true
                    horizontalAlignment:Text.AlignHCenter
                }

                BreathChartPane {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    profile:root.profile
                    breathSurvey:root.breathSurvey
                }
            }
        }
    }


}
