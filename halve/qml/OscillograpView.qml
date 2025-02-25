import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtCharts 2.5
import QtQuick.Dialogs
import "utils.js" as Utils
import "models"

Rectangle {
    id:root
    required property var profile
    required property var channel
    required property var breathSurvey
    property var options: root.profile.oscillograpOptions
    property var mappingDetection
    required property var catheterListModel;
    property int samplingRate: AppSettings.samplingRate
    property int oscillograpScaleplateStep: AppSettings.oscillograpScaleplateStep
    readonly property real dpi: Screen.pixelDensity * 25.4

    color:"#191919"
    clip: true

    OscillograpChartListModel {
        id:currentChannelListModel
        profile:root.profile
    }

    ChannelDataFilter {
        id:currentChannelDataFilter
        samplingRate:AppSettings.samplingRate
        profile:root.profile
    }
    OscillograpDataSource{
        id:dataSource
        profile:root.profile
        channel:root.channel
        breathSurvey:root.breathSurvey
        channelDataFilter:currentChannelDataFilter
        onCurrentIndexChanged: {
            eraseLine.x = scaleplate.x + currentIndex * (scaleplate.width / dataSource.max);
        }
        property var profileType:root.profile.type
        onProfileTypeChanged: {
            if (dataSource.profileType === Halve.PROFILE_SNAPSHOT) {
                dataSource.mode = OscillograpDataSource.FORWARD;
            } else {
                dataSource.mode = OscillograpDataSource.LOOP;
            }
        }

        onPausedChanged: {
            if (dataSource.profileType === Halve.PROFILE_SNAPSHOT) {
                if (dataSource.paused) {
                    root.channel.lookbackSpeed = 0;
                } else {
                    root.channel.lookbackSpeed = root.options.lookbackSpeed;
                }
            }
        }

        onChannelChanged: {
            if (dataSource.profileType === Halve.PROFILE_SNAPSHOT) {
                var snapshot = root.profile.snapshot;
                root.channel.lookback(snapshot.beginTime, snapshot.endTime);
            }
        }
    }
    onSamplingRateChanged: scaleplateTimer.restart()
    onOscillograpScaleplateStepChanged: scaleplateTimer.restart()

    function calculateMax() {
        var scaleplateWidth = scaleplate.width / dpi * 25.4;
        var dataSourceMax = scaleplateWidth / options.oscillograpSpeed * AppSettings.samplingRate;
        dataSource.max = dataSourceMax;
        var step = dataSource.max / (scaleplateWidth / AppSettings.oscillograpScaleplateStep);
        if (step <= 0) {
            return;
        }
        scaleplate.step =  Math.floor(step / 100) * 100
    }

    Component {
        id:chartListLoader
        OscillograpChartList {
            id:oscillograpChartList
            max: dataSource.max
            options:root.profile.oscillograpOptions
            source:dataSource
            model:currentChannelListModel
        }
    }

    function reinitChartList() {
        chartListLoaderStackView.replace(chartListLoader);
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        RowLayout {
            id:toolbar
            Layout.fillWidth: true
            Layout.maximumHeight:  34
            Layout.rightMargin: 3
            Layout.leftMargin: 3
            ComboBox {
                textRole: "text"
                valueRole: "value"
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: 100
                model: SpeedModel{}
                onActivated: {
                    options.oscillograpSpeed = currentValue;
                    scaleplateTimer.restart();
                    options.rulerType = 0;
                }
                Component.onCompleted: currentIndex = indexOfValue(options.oscillograpSpeed)
            }
            ToolSeparator {}
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: dataSource.paused?"qrc:/assets/images/play.png":"qrc:/assets/images/pause.png"
                    height: 19
                    width:19
                }
                onClicked: {
                    dataSource.paused = !dataSource.paused;
                }

                Timer {
                    id:statusTimer
                    repeat: false
                    interval: 500
                    onTriggered: dataSource.paused = false
                }
                Component.onCompleted: statusTimer.start()

                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Waveform display settings")
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: dataSource.mode == OscillograpDataSource.LOOP?"qrc:/assets/images/loop.png":"qrc:/assets/images/forward.png"
                    height: 19
                    width:19
                }
                onClicked: {
                    dataSource.paused = false;
                    dataSource.mode = (dataSource.mode == OscillograpDataSource.LOOP?OscillograpDataSource.FORWARD:OscillograpDataSource.LOOP);
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Erase mode")
            }
            ToolSeparator{}

            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/filter.png"
                    height: 19
                    width:19
                }
                onClicked: filterSettingPopup.open()
                Menu {
                    id: filterSettingPopup
                    y: parent.height
                    contentItem: Pane {
                        focusPolicy: Qt.ClickFocus
                        RowLayout {
                            anchors.fill: parent
                            FilterSettingPane {
                                filterOptions:root.profile.filterOptions
                            }
                        }
                    }
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Filter Settings")
            }

            ToolButton {

                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/sign.png"
                    height: 19
                    width:19
                }
                onClicked: signSettingPopup.open();
                Menu {
                    id: signSettingPopup
                    y: parent.height
                    contentItem: Pane {
                        focusPolicy: Qt.ClickFocus
                        RowLayout {
                            anchors.fill: parent
                            OscillograpSignSettingPane {
                                profile:root.profile
                                catheterListModel:root.catheterListModel
                            }
                        }
                    }
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Signal Settings")
            }
            ToolSeparator {}



            ToolButton {
                Layout.alignment: Qt.AlignHCenter
                icon {
                    source: "qrc:/assets/images/ruler_lat.png"
                    height: 19
                    width:19
                }

                checked: options.rulerType === 1
                onClicked: {
                    options.rulerType = (options.rulerType === 1?0:1)
                }


                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Ruler of LAT")
            }

            ToolButton {
                Layout.alignment: Qt.AlignHCenter
                icon {
                    source: "qrc:/assets/images/ruler_voltage.png"
                    height: 19
                    width:19
                }
                checked: options.rulerType === 2
                onClicked: {
                    options.rulerType = (options.rulerType === 2?0:2)
                }

                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Ruler of voltage")
            }
            ToolButton {
                Layout.alignment: Qt.AlignHCenter
                icon {
                    source: "qrc:/assets/images/sort.svg"
                    height: 19
                    width:19
                }
                onClicked: {
                    options.sortRole = (options.sortRole === OscillograpOptions.ASC?OscillograpOptions.DESC:OscillograpOptions.ASC)
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Sort")
            }

            ToolButton {
                Layout.alignment: Qt.AlignHCenter
                icon {
                    source: "qrc:/assets/images/regulate.svg"
                    height: 19
                    width:19
                }

                onClicked: regulateSettingPopup.open();
                Menu {
                    id: regulateSettingPopup
                    y: parent.height
                    contentItem: Pane {
                        focusPolicy: Qt.ClickFocus
                        RegulateSettingPane {
                            anchors.fill: parent
                            setting:root.profile.oscillograpOptions
                        }
                    }
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Regulate")
            }
            ToolSeparator{}

            // ToolButton {

            //     Layout.alignment: Qt.AlignVCenter
            //     icon {
            //         source: "qrc:/assets/images/settings.png"
            //         height: 19
            //         width:19
            //     }
            //     onClicked: settingsSettingPopup.open();
            //     Menu {
            //         id: settingsSettingPopup
            //         y: parent.height
            //         contentItem: Pane {
            //             focusPolicy: Qt.ClickFocus
            //             ColumnLayout {
            //                 anchors.fill: parent
            //                 spacing: 7
            //                 CheckBox {
            //                     id:showAxesCheckbox
            //                     checked: options.oscillograpAxisLine
            //                     text:qsTr("Visibility of the axis grid line")
            //                     onClicked: options.oscillograpAxisLine = checked
            //                 }
            //             }
            //         }
            //     }
            //     hoverEnabled: true
            //     ToolTip.visible: hovered
            //     ToolTip.text: qsTr("Settings")
            // }
        }

        Rectangle {
            id:channelChartView
            Layout.fillWidth: true
            Layout.fillHeight: true
            color:"black"
            ColumnLayout {
                spacing: 0
                anchors.fill: parent
                RowLayout {
                    spacing: 0
                    Layout.fillWidth: true
                    Layout.maximumHeight: 20
                    Label {
                        text:""
                        horizontalAlignment:Text.AlignRight
                        font.pixelSize: 13
                        font.bold: true
                        Layout.fillHeight: true
                        verticalAlignment : Text.AlignVCenter
                        Layout.preferredWidth: AppSettings.channelChartListLabelWidth
                        rightPadding: 10
                    }

                    Scaleplate {
                        id:scaleplate
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        maxValue:Math.floor(dataSource.max / AppSettings.samplingRate * 1000)
                        step:100
                        onWidthChanged: scaleplateTimer.restart()

                        Timer {
                            id:scaleplateTimer
                            interval: 300; running: false; repeat: false
                            onTriggered: calculateMax()
                        }
                    }
                }
                StackView {
                    id:chartListLoaderStackView
                    Layout.fillWidth: true
                    Layout.fillHeight:true
                    initialItem: chartListLoader
                    replaceEnter:Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 0
                            to:1
                            duration: 200
                        }
                    }

                    replaceExit: Transition {
                        PropertyAnimation {
                            property: "opacity"
                            from: 1
                            to:0
                            duration: 200
                        }
                    }
                }
                HorizontalScrollBar {
                    id: hbar
                    visible: dataSource.profileType === Halve.PROFILE_SNAPSHOT
                    Layout.fillWidth: true
                    Layout.leftMargin: 10
                    Layout.rightMargin: 5
                    Layout.preferredHeight: 20
                    to:channel.size
                    value:channel.index
                    onMoved: {
                        dataSource.paused = true;
                        root.channel.ship(value, dataSource.max);
                    }
                }
            }

            Rectangle {
                id:eraseLine
                height: parent.height
                width: 10
                color: "black"
                y:0
                x:0
                visible: dataSource.mode === OscillograpDataSource.LOOP
            }
        }
    }


}
