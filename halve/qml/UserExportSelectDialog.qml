import QtQuick
import Qt.labs.platform  as Platform
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels
import "models"


Dialog {
    id:root
    property alias model: searchModel.sourceModel
    anchors.centerIn:parent
    title: "导出病例"
    standardButtons: Dialog.Ok | Dialog.Cancel
    UserListSortFilterProxyModel {
        id:searchModel
    }
    signal selected(var path, var userIds);

    contentItem: Rectangle {
        color: "#2b2b2b"
        implicitWidth: 400
        implicitHeight: 400
        ButtonGroup {
             id: childGroup
             exclusive: false
        }
        ColumnLayout {
            anchors.fill: parent
            RowLayout {
                Layout.fillWidth: true
                TextField {
                    id:searchTextField
                    background: Rectangle {
                        border.width: 0
                        color: "black"
                    }

                    placeholderText:"关键词搜索"
                    placeholderTextColor:"white"
                    Layout.fillWidth: true
                    Layout.margins: 1
                    onTextEdited: {
                        searchModel.setFilterFixedString(this.text);
                    }
                }
            }


            Rectangle {
                id: userList
                property alias currentRow:tableView.currentRow
                color:"black"

                Layout.fillHeight: true
                Layout.fillWidth: true
                SexModel {
                    id:sexModel
                }

                ColumnLayout {
                    id: tableViewLayout
                    anchors.fill: parent
                    spacing:0

                    HorizontalHeaderView {
                        id: headerView
                        readonly property var columnNames: [
                            qsTr("Check"),
                            qsTr("ID"),
                            qsTr("Name"),
                            qsTr("Sex"),]
                        syncView: tableView
                        Layout.fillWidth: true
                        flickableDirection:Flickable.AutoFlickIfNeeded
                        resizableColumns: false // otherwise QTBUG-111013 happens
                        delegate: DelegateChooser {
                            id: chooser
                            DelegateChoice {
                                column: 0

                                delegate:Rectangle {
                                    required property int index
                                    color:"#333333"
                                    implicitHeight: 30


                                }
                            }
                            DelegateChoice {
                                column: 1
                                delegate:Rectangle {
                                    required property int index
                                    color:"#333333"
                                    implicitHeight: 30
                                    Text {
                                        color:"#e7e7e7"
                                        font.pixelSize: 14
                                        anchors.fill: parent
                                        text: headerView.columnNames[index]
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                            DelegateChoice {
                                column: 2
                                delegate:Rectangle {
                                    required property int index
                                    color:"#333333"
                                    implicitHeight: 30
                                    Text {
                                        color:"#e7e7e7"
                                        font.pixelSize: 14
                                        anchors.fill: parent
                                        text: headerView.columnNames[index]
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                            DelegateChoice {
                                column: 3
                                delegate:Rectangle {
                                    required property int index
                                    color:"#333333"
                                    implicitHeight: 30
                                    Text {
                                        color:"#e7e7e7"
                                        font.pixelSize: 14
                                        anchors.fill: parent
                                        text: headerView.columnNames[index]
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }

                        }
                    }

                    TableView {
                        id: tableView
                        model: searchModel
                        alternatingRows:true
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        columnWidthProvider: function (column) {
                            if (column === 0) {
                                return 50;
                            } else {
                                return (tableView.width - 50) / (headerView.columnNames.length - 1);
                            }

                        }
                        rowHeightProvider:function(){return 30;}
                        flickableDirection:Flickable.AutoFlickIfNeeded

                        selectionModel: ItemSelectionModel {}

                        reuseItems: false
                        clip: true

                        delegate: DelegateChooser {
                            DelegateChoice {
                                column: 0
                                delegate:Rectangle {
                                    color: row === tableView.currentRow? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                                    CheckBox {
                                        property var guid: id
                                        anchors.centerIn: parent
                                        ButtonGroup.group: childGroup
                                    }
                                }
                            }
                            DelegateChoice {
                                column: 1

                                delegate:Rectangle {
                                    color: row === tableView.currentRow? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                                    Text {
                                        anchors.fill: parent
                                        anchors.margins: 5
                                        elide:Text.ElideRight
                                        text: idcard
                                        font.pixelSize: 15
                                        color:"white"
                                        horizontalAlignment: Text.AlignLeft
                                        verticalAlignment:Text.AlignVCenter
                                    }

                                }
                            }
                            DelegateChoice {
                                column: 2
                                delegate:Rectangle {
                                    color: row === tableView.currentRow ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                                    Text {
                                        anchors.fill: parent
                                        anchors.margins: 5
                                        elide:Text.ElideRight
                                        text: name
                                        font.pixelSize: 15
                                        color:"white"
                                        horizontalAlignment: Text.AlignLeft
                                        verticalAlignment:Text.AlignVCenter
                                    }

                                }
                            }
                            DelegateChoice {
                                column: 3

                                delegate:Rectangle {
                                    color: row === tableView.currentRow? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                                    Text {
                                        anchors.fill: parent
                                        anchors.margins: 5
                                        elide:Text.ElideRight
                                        text:formatText(sex)
                                        function formatText(v) {
                                            return sexModel.getTextByValue(v)
                                        }
                                        font.pixelSize: 15
                                        color:"white"
                                        horizontalAlignment: Text.AlignLeft
                                        verticalAlignment:Text.AlignVCenter
                                    }
                                }
                            }

                        }
                    }

                }
            }
        }
    }
    Platform.FolderDialog {
        id: fileExportDialog;
        property var exportGuids: []
        title: qsTr("Please choose a file");
        onAccepted: {
            var fileUrl = new String(fileExportDialog.folder);
            root.selected(fileUrl.substring(8), fileExportDialog.exportGuids);
        }
    }

    onAccepted: {
        fileExportDialog.exportGuids = [];
        for(var i = 0; i < childGroup.buttons.length; ++i) {
            if (childGroup.buttons[i].checked) {
                fileExportDialog.exportGuids.push(childGroup.buttons[i].guid);
            }
        }
        if (fileExportDialog.exportGuids.length > 0) {
            fileExportDialog.open();
        }
    }
}
