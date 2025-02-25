import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import QtQuick as QtQ

Rectangle {
    id: root
    property alias model:listView.model
    property int currentRow:-1
    property alias count:listView.count
    property var buttonGroup
    readonly property int itemHeight: 30
    color:"#333333"
    function scrollTo(idx) {
        root.currentRow = idx;
        if (idx >= 0 && idx < listView.count) {
            flickable.contentY = idx * itemHeight
            flickable.returnToBounds();
        }
    }
    Connections {
        target: root.model
        function onRowsRemoved(){
            root.currentRow = -1;
        }
        function onRowsInserted(){
            root.currentRow = -1;
        }
    }
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        RowLayout {
            Layout.fillWidth: true
            Layout.maximumHeight: 25
            spacing: 0
            Label {
                Layout.preferredWidth: 30
                Layout.fillHeight: true
                text:qsTr("")
            }
            Label {
                Layout.preferredWidth: 40
                Layout.fillHeight: true
                text:qsTr("Appeared")
                horizontalAlignment:Text.AlignHCenter
                verticalAlignment:Text.AlignVCenter
            }
            Label {
                Layout.preferredWidth: 50
                Layout.fillHeight: true
                text:qsTr("Color")
                horizontalAlignment:Text.AlignHCenter
                verticalAlignment:Text.AlignVCenter
            }
            Label {
                Layout.preferredWidth: 40
                Layout.fillHeight: true
                text:qsTr("Order")
                horizontalAlignment:Text.AlignHCenter
                verticalAlignment:Text.AlignVCenter
            }
            Label {
                Layout.preferredWidth: 40
                Layout.fillHeight: true
                text:qsTr("Prompt")
                horizontalAlignment:Text.AlignHCenter
                verticalAlignment:Text.AlignVCenter
            }
            Label {
                Layout.fillWidth: true
                Layout.fillHeight: true
                horizontalAlignment:Text.AlignHCenter
                verticalAlignment:Text.AlignVCenter
                text:qsTr("Description")
            }
        }
        Flickable {
           id: flickable
           clip: true
           Layout.fillWidth: true
           Layout.fillHeight: true
           contentWidth: root.width
           contentHeight: repeaterItem.height
           rebound: Transition {
                NumberAnimation {
                    properties: "x,y"
                    duration: 0
                    easing.type: Easing.OutBounce
                }
            }
           Column {
               id: repeaterItem
               width: flickable.width
                Repeater {
                    id: listView
                    delegate: Rectangle {
                        width: parent.width
                        height: root.itemHeight
                        color:root.currentRow == index? "#46607c" : (index %2==0?"#2b2b2b":"#282828")
                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            spacing: 0
                            Item {
                                Layout.preferredWidth: 30
                                Layout.fillHeight: true
                                CheckBox {
                                    property var rowId: id
                                    anchors.centerIn: parent
                                    ButtonGroup.group: root.buttonGroup
                                }
                            }
                            Item {
                                Layout.preferredWidth: 40
                                Layout.fillHeight: true
                                ApparentSwitch {
                                    checked: appeared
                                    anchors.centerIn: parent
                                    onClicked: {
                                        root.model.setData(root.model.index(index,0), checked, DissolveListModel.APPEARED);
                                    }
                                }
                            }
                            Item {
                                Layout.preferredWidth: 50
                                Layout.fillHeight: true
                                ColorCarp{
                                    id:colorPicker
                                    color:dyestuff
                                    anchors.fill: parent
                                    onAccepted: root.model.setData(root.model.index(index,0), colorPicker.color, DissolveListModel.DYESTUFF)
                                }
                            }
                            Item {
                                Layout.preferredWidth: 40
                                Layout.fillHeight: true
                                Text {
                                    anchors.centerIn: parent
                                    text: id
                                    font.pixelSize: 15
                                    color:"white"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment:Text.AlignVCenter
                                }
                            }
                            Item {
                                Layout.preferredWidth: 40
                                Layout.fillHeight: true
                                CheckBox {
                                    checked: prompt
                                    anchors.centerIn: parent
                                    onClicked: {
                                        root.model.setData(root.model.index(index,0), checked, DissolveListModel.PROMPT);
                                    }
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                Text {
                                    anchors.fill: parent
                                    elide:Text.ElideRight
                                    text: description
                                    font.pixelSize: 15
                                    color:"white"
                                    horizontalAlignment: Text.AlignLeft
                                    verticalAlignment:Text.AlignVCenter
                                }
                            }

                            QtQ.TapHandler {
                                onTapped: {
                                    root.currentRow = index;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
