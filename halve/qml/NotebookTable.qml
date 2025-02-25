import QtQuick 2.0
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels
import "utils.js" as Utils
import QtQuick as QtQ


Rectangle {
    id: root
    property alias model:listView.model
    property int currentRow:-1
    property alias count:listView.count
    readonly property int itemHeight: 30
    color:"#333333"

    signal doubleTapped();
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
        Flickable {
           id: flickable
           clip: true
           Layout.fillWidth: true
           Layout.fillHeight: true
           contentWidth: root.width
           contentHeight: repeaterItem.height
           Column {
               id: repeaterItem
               width: flickable.width
                Repeater {
                    id: listView
                    delegate: Rectangle {
                        width: parent.width
                        height: root.itemHeight
                        color:root.currentRow == index? "#46607c" : index %2==0?"#2b2b2b":"#282828"
                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            spacing: 2

                            Image {
                                id:icon
                                Layout.preferredWidth: 17
                                Layout.preferredHeight: 17
                                Component.onCompleted: {
                                    icon.source = Utils.getNotebookItemIcon(type);
                                }
                            }

                            Label {
                                elide:Text.ElideRight
                                text: name
                                font.pixelSize: 13
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment:Text.AlignVCenter
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            Label {
                                text:createTime.toLocaleString(locale, "hh:mm:ss")
                                font.pixelSize: 13
                                color:"lightyellow"
                            }
                            QtQ.TapHandler {
                                onTapped: {
                                    root.currentRow = index;
                                }
                                onDoubleTapped: {
                                    root.doubleTapped();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
