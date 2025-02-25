import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtCharts 2.5

Item {
    id:root
    required property var max
    required property var source
    required property var options
    property alias  model:delegateModel.model
    property alias  count:listView.count

    property int lastHeight: 0

    Component {
        id: dragDelegate
        Item {
            id: dragArea

            property bool held: false
            required property var index
            required property var name
            required property var id
            required property var type
            required property var basis
            required property var dyestuff
            required property var surveyor
            property real uniqueId: Math.random()
            onDyestuffChanged: {
                colorPicker.color = dyestuff;
            }

            onSurveyorChanged: {
                surveyorLine.value = surveyor;
            }
            anchors {
                left: parent?.left
                right: parent?.right
            }
            height: content.height

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

                radius: 2
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
                            wrapMode: Text.Wrap
                            font.pointSize: 8

                            MouseArea {
                                drag.target: held ? content : undefined
                                drag.axis: Drag.YAxis
                                anchors.fill: parent
                                onPressed: {
                                    held = true
                                }
                                onReleased: {
                                    held = false
                                }
                                onCanceled: {
                                    held = false
                                }
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
                                root.model.setData(root.model.index(index, 0), colorPicker.color, OscillograpChartListModel.DYESTUFF)
                            }
                        }
                    }
                    property var basis:dragArea.basis === -1?(dragArea.type === Halve.ECG?AppSettings.ecgBasis:AppSettings.basis):dragArea.basis
                    onBasisChanged: resizeBasisHeight()

                    function resizeBasisHeight() {
                        var j = channelChart.height/surveyorLine.height;
                        var j2 = basis/2;
                        channelChart.basisHeight = j * j2;
                    }

                    ChannelChart {
                        id:channelChart
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        property int basisHeight:basis/2

                        color:dyestuff
                        max:root.max
                        useOpenGL: true
                        ymin: -basisHeight
                        ymax: basisHeight
                        tickCount:10
                        Component.onCompleted: {
                            source.setSeries(uniqueId, model.get(index), this.series(0))
                        }
                        Component.onDestruction: {
                            source.setSeries(uniqueId, null, null)
                        }
                        onHeightChanged:parent.resizeBasisHeight()
                        SurveyorLine {
                            id:surveyorLine
                            enabled: type !== ElectrodeNode.PANT
                            visible: type !== ElectrodeNode.PANT
                            anchors.verticalCenter: parent.verticalCenter
                            chartHeight:channelChart.height
                            onHeightChanged:{
                                parent.parent.resizeBasisHeight()
                                root.model.setData(root.model.index(index, 0), surveyorLine.value, OscillograpChartListModel.SURVEYOR)
                            }
                            value:dragArea.surveyor
                        }

                        Loader {
                            active: options.rulerType !== 0
                            anchors.fill: parent
                            asynchronous: true
                            visible: status == Loader.Ready
                            sourceComponent: RulerPane {
                                anchors.fill: parent
                                type:options.rulerType
                                regulate:surveyorLine.idx
                                latBasis:root.max
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
        flickableDirection: Flickable.AutoFlickIfNeeded
        anchors {
            fill: parent
        }
        spacing:0
        model: delegateModel
    }


}

