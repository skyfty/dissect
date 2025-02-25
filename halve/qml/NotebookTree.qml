import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels
import "utils.js" as Utils
import QtQuick

Rectangle {
    id: root
    property alias model:treeView.model
    property var currentIndex: treeView.index(treeView.currentRow, 0)
    color:"#333333"
    signal doubleTapped();

    Connections {
        target: treeView.model
        function onRowsRemoved() {
            treeView.closeEditor()
            treeView.selectionModel.clearCurrentIndex();
        }
        function onRowsInserted() {
            treeView.closeEditor();
        }
    }

    Component {
        id:editDialogComponent
        EditForm {
            id:editForm
            property int row:-1
            property alias text:nameTextInput.text

            maximumLength: 20
            onAccepted: {
                var nameVal = text.trim();
                if (nameVal === "") {
                    return;
                }
                var itemIndex = treeView.index(row, 0);
                var idx = root.model.indexOfName(itemIndex, nameVal);
                if (idx !== -1) {
                } else {
                    treeView.model.setData(itemIndex, nameVal, NotebookTreeModel.NAME)
                }
                editForm.destroy()
            }
            contentItem: TextInput {
                id:nameTextInput
                verticalAlignment:Text.AlignVCenter
                color: "white"
                onAccepted: {
                    editForm.close();
                }
                focus: true
            }
        }
    }





    TreeView  {
        id: treeView

        anchors.fill: parent

        flickableDirection: Flickable.AutoFlickIfNeeded
        clip: true
        reuseItems:false
        alternatingRows:true
        selectionModel: ItemSelectionModel {}
        rowHeightProvider:function(){return 30;}
        columnWidthProvider: function() { return width }
        Component.onCompleted: treeView.expand(0)
        delegate: TreeViewDelegate {
            id: treeDelegate
            required property int index
            required property string name
            required property var id

            topPadding:0

            background: Rectangle {
                color: row === treeView.currentRow ? "#46607c" :  (index %2==0?"#2b2b2b":"#282828")
            }
            contentItem: Item {
                RowLayout {
                    anchors.fill: parent
                    Row {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        spacing: 3
                        Image {
                            id:icon
                            height: 16
                            width:16
                            anchors.verticalCenter: parent.verticalCenter
                            Component.onCompleted: {
                                icon.source = Utils.getNotebookItemIcon(type, id);
                            }
                        }

                        Label {
                            id:nameLabel
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.width - icon.width
                            text: qsTr(name)
                            verticalAlignment:Text.AlignVCenter
                            height: parent.height -5
                            color: "white"
                        }

                        TapHandler {
                            onTapped: {
                                treeView.selectionModel.setCurrentIndex(treeView.index(row, 0), ItemSelectionModel.SelectCurrent)
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
