import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtCharts 2.5

Item {
    id:root
    required property var setting
    required property var source
    property alias model:delegateModel.model
    property alias count:listView.count
    property var ymin
    property var ymax
    required property var profile
    property var mappingType:root.profile.mappingType
    property int currentRowId:-1

    Component {
        id: dragDelegate
        Item {
            id: dragArea

            property bool held: false
            required property var index
            required property var name
            required property var id
            required property var type
            required property var dyestuff
            required property var surveyor


            onSurveyorChanged: {
                surveyorLine.value = surveyor;
            }
            onDyestuffChanged: {
                colorPicker.color = dyestuff;
            }
            anchors {
                left: parent?.left
                right: parent?.right
            }
            height: content.height

            Rectangle {
                anchors.fill: parent
                border.color: "lightgray"
                opacity: 0.2
                visible: root.currentRowId === meltSampleLine.rowId
            }

            Rectangle {
                id: content
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                width: dragArea.width
                height: root.height / listView.count
                border.width:dragArea.held ? 1:0
                border.color: dragArea.held ? "lightsteelblue":"transparent"
                color:dragArea.held ? Qt.rgba(128, 128, 128, 0.2):"transparent"
                Behavior on border.color { ColorAnimation { duration: 100 } }
                radius: dragArea.held ? 2:0
                Drag.active: dragArea.held
                Drag.source: dragArea
                Drag.hotSpot.x: width / 2
                Drag.hotSpot.y: height / 2
                states: State {
                    when: dragArea.held

                    ParentChange {
                        target: content
                        parent: root
                    }
                    AnchorChanges {
                        target: content
                        anchors {
                            horizontalCenter: undefined
                            verticalCenter: undefined
                        }
                    }
                }
                RowLayout {
                    spacing: 0
                    anchors.fill: parent
                    Control {
                        id:nameControl
                        Layout.maximumWidth: AppSettings.channelChartListLabelWidth
                        Layout.minimumWidth: AppSettings.channelChartListLabelWidth
                        Layout.fillHeight: true
                        Layout.margins:0
                        Label {
                            verticalAlignment : Text.AlignVCenter
                            horizontalAlignment:Text.AlignRight
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.fill: parent
                            rightPadding: 10
                            color:dyestuff
                            text: name
                            wrapMode: Text.WordWrap
                            font.pointSize: 8
                            font.bold: root.currentRowId === meltSampleLine.rowId

                            MouseArea {
                                drag.target: held ? content : undefined
                                drag.axis: Drag.YAxis
                                anchors.fill: parent
                                onPressAndHold: held = true
                                onReleased: held = false
                            }
                        }
                        ColorCarp{
                            id:colorPicker
                            color:dyestuff
                            visible: nameControl.hovered
                            width:15
                            height: 15
                            x:10
                            radius: 5
                            anchors.verticalCenter: parent.verticalCenter
                            onAccepted: {
                                root.model.setData(root.model.index(index, 0), colorPicker.color, MeltChannelChartListModel.DYESTUFF)
                            }
                        }
                    }
                    property var basis:dragArea.type === Halve.ECG?AppSettings.ecgBasis:AppSettings.basis

                    function resizeBasisHeight() {
                        var j = channelChart.height/surveyorLine.height;
                        var j2 = basis/2;
                        channelChart.basisHeight = j * j2;
                    }

                    ChannelChart {
                        id:channelChart
                        property string uniqueId: Math.random().toString()
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        property real basisHeight:basis/2
                        color:dyestuff
                        max:setting.max
                        useOpenGL: true
                        ymin: -basisHeight
                        ymax: basisHeight
                        Component.onCompleted: {
                            source.setSeries(uniqueId, model.get(index), this.series(0), meltSampleLine)
                        }
                        Component.onDestruction: {
                            source.setSeries(uniqueId, null, null, null)
                        }
                        onHeightChanged: {
                            parent.resizeBasisHeight();
                        }
                        onWidthChanged:meltSampleLine.place()

                        SurveyorLine {
                            id:surveyorLine
                            y:0
                            x:0
                            idx:AppSettings.defaultSurveyor
                            anchors.verticalCenter: parent.verticalCenter
                            chartHeight:channelChart.height
                            onHeightChanged:{
                                parent.parent.resizeBasisHeight()
                                root.model.setData(root.model.index(index, 0), surveyorLine.value, MeltChannelChartListModel.SURVEYOR)
                            }
                            value:dragArea.surveyor
                        }


                        MeltSampleLine {
                            id:meltSampleLine
                            property var mappingType:root.profile.mappingType
                            height: Math.min(parent.height, surveyorLine.height)
                            width: 7
                            anchors.verticalCenter: parent.verticalCenter
                            function place(seriesIndex) {
                                if (seriesIndex === -1) {
                                    var ratioInWidth= setting.max / (root.width - AppSettings.channelChartListLabelWidth);
                                    var middleX = setting.baseIndex / ratioInWidth;
                                    meltSampleLine.x = middleX - 10;
                                } else {
                                    meltSampleLine.x = seriesIndex * (parent.width / setting.max) - meltSampleLine.width / 2;
                                }
                            }
                            visible: x > 0
                            onLatSeriesIndexChanged:  {
                                if (mappingType === Halve.Lat) {
                                    place(latSeriesIndex);
                                }
                            }
                            onVoltageSeriesIndexChanged:  {
                                if (mappingType === Halve.Voltage) {
                                    place(voltageSeriesIndex);
                                }
                            }
                            onMappingTypeChanged: {
                                if (mappingType === Halve.Lat) {
                                    place(latSeriesIndex);
                                } else {
                                    place(voltageSeriesIndex);
                                }
                            }

                            Rectangle{
                                id:biaogan
                                width: 2
                                height: parent.height
                                anchors.horizontalCenter: parent.horizontalCenter
                                color: meltSampleLine.index === -1?"red":"lightyellow"
                            }

                            Label {
                                id:sampleLabel
                                anchors.bottom: parent.bottom
                                anchors.left: biaogan.right
                                anchors.leftMargin: 3
                                font.pixelSize: 12
                                font.weight: Font.Bold
                                color:"yellow"
                                function formatText() {
                                    if (meltSampleLine.mappingType === Halve.Voltage) {
                                        return meltSampleLine.voltageSeriesIndex === -1?
                                                    "???":
                                                    ((Math.max(meltSampleLine.maxVoltage - meltSampleLine.minVoltage, 0) / AppSettings.basis).toFixed(1) + " mV");
                                    } else {
                                        return meltSampleLine.latSeriesIndex === -1?
                                                    "???":
                                                    (Math.floor(meltSampleLine.lat / AppSettings.samplingRate * 1000) + " ms");
                                    }
                                }
                                text:formatText()
                            }
                            MouseArea {
                                property var persistX;
                                enabled: root.profile.meltState !== Halve.TADK
                                anchors.fill: parent
                                onPressed: {
                                    persistX = mouseX
                                }
                                onReleased: {
                                    meltSampleLine.adjust = true;
                                }

                                cursorShape: Qt.SizeHorCursor
                                onPositionChanged: {
                                    var ratioInWidth= setting.max / (root.width - AppSettings.channelChartListLabelWidth);
                                    var leftAreaX  = setting.leftIndex / ratioInWidth;
                                    var rightAreaX = setting.rightIndex / ratioInWidth;
                                    var middleX = setting.baseIndex / ratioInWidth;
                                    var rx = meltSampleLine.x + mouseX - persistX;
                                    if (rx < 0) {
                                        return;
                                    }
                                    var meltSampleLineMidle = meltSampleLine.width / 2;
                                    if (rx < leftAreaX - meltSampleLineMidle) {
                                        rx = leftAreaX - meltSampleLineMidle;
                                    } else {
                                        if (rx + meltSampleLineMidle > rightAreaX) {
                                            rx = rightAreaX - meltSampleLineMidle;
                                        }
                                    }
                                    var idx = (rx  + meltSampleLineMidle) / (parent.parent.width / setting.max);
                                    source.setSampleLineIndex(channelChart.uniqueId, idx);
                                }
                            }
                        }

                        Loader {
                            active: setting.rulerType !== 0
                            anchors.fill: parent
                            asynchronous: true
                            visible: status == Loader.Ready
                            sourceComponent: MeltRulerPane {
                                anchors.fill: parent
                                type:setting.rulerType
                                latBasis: setting.max
                                regulate:surveyorLine.idx
                            }
                        }
                    }
                }
            }
            DropArea {
                anchors.fill: parent
                onEntered: (drag) => {
                   delegateModel.items.move(drag.source.DelegateModel.itemsIndex,dragArea.DelegateModel.itemsIndex)
               }
            }

        }
    }

    DelegateModel {
        id: delegateModel
        delegate: dragDelegate
    }

    ListView {
        id:listView
        anchors {
            fill: parent
        }
        flickableDirection: Flickable.AutoFlickIfNeeded
        clip: true
        spacing:0
        cacheBuffer: 50
        model: delegateModel
        moveDisplaced: Transition {
             NumberAnimation { properties: "x,y"; duration: 300 }
        }
    }

}

