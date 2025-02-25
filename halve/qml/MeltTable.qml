
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels
import Qt.labs.platform

Rectangle {
    id: mainview
    property alias model:tableView.model

    property int currentRow:-1
    color:"#333333"

    ColumnLayout {
        id: tableViewLayout
        anchors.fill: parent
        spacing:0

        HorizontalHeaderView {
            id: headerView
            readonly property var columnNames: ["显示","顺序","治疗描述","直径","展示病灶说明","颜色"]
            syncView: tableView
            Layout.fillWidth: true
            flickableDirection:Flickable.AutoFlickIfNeeded
            resizableColumns: false // otherwise QTBUG-111013 happens
            delegate: Rectangle{
                required property int index
                color:"#333333"
                implicitHeight: 30
                Text {
                    color:"#e7e7e7"
                    font.pixelSize: 14
                    font.bold: true
                    anchors.leftMargin: 4
                    anchors.fill: parent
                    text: headerView.columnNames[index]
                    horizontalAlignment: index === 1 ?Text.AlignLeft:Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        Connections {
            target: tableView.model
            onRowsRemoved: {
                tableView.selectionModel.clearCurrentIndex();
            }
        }
        TableView {
            id: tableView
            alternatingRows:true
            Layout.fillHeight: true
            Layout.fillWidth: true
            columnWidthProvider: function () { return tableView.width / tableView.model.columnCount(); }
            rowHeightProvider:function(){return 30;}
            flickableDirection:Flickable.AutoFlickIfNeeded
            ScrollBar.vertical: ScrollBar {
               parent: tableView.parent
               anchors.top: tableView.top
               anchors.bottom: tableView.bottom
               anchors.left: tableView.right
            }

            selectionModel: ItemSelectionModel {
                onCurrentChanged:currentRow = currentIndex.row
            }
            reuseItems: false
            clip: true

            delegate: DelegateChooser {
                id: chooser
                DelegateChoice  {
                    column: 0
                    delegate:Rectangle {
                        color: row === tableView.selectionModel.currentIndex.row ? "#46607c" : (row %2==0?"#2b2b2b":"#282828")
                        CheckBox {
                            anchors.centerIn: parent
                            checked: appeared
                            onClicked: {
                                let modelIndex = tableView.index(row, 0);
                                tableView.model.setData(modelIndex, checked, MeltTableModel.Appeared);
                                tableView.selectionModel.setCurrentIndex(modelIndex, ItemSelectionModel.SelectCurrent)
                            }
                        }
                    }
                }

                DelegateChoice {
                    column: 1
                    delegate:Rectangle {
                        color: row === tableView.selectionModel.currentIndex.row ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text: order
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                            Control {
                                id:orderToolTipControl
                                anchors.fill: parent
                            }
                            ToolTip.visible: orderToolTipControl.hovered
                            ToolTip.text: text
                        }
                    }
                }
                DelegateChoice {
                    column: 2
                    delegate:Rectangle {
                        color: row === tableView.selectionModel.currentIndex.row ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text: description
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                            Control {
                                id:descriptionToolTipControl
                                anchors.fill: parent
                            }
                            ToolTip.visible: descriptionToolTipControl.hovered
                            ToolTip.text: text
                        }

                        TableView.editDelegate: TextField {
                            anchors.margins: 1
                            anchors.fill: parent
                            text: description
                            font.pixelSize: 15
                            horizontalAlignment: TextInput.AlignLeft
                            verticalAlignment: TextInput.AlignVCenter
                            TableView.onCommit:  tableView.model.setData(tableView.index(row, column), text, MeltTableModel.Description)

                        }
                    }
                }
                DelegateChoice {
                    column: 3

                    delegate:Rectangle {
                        color: row === tableView.selectionModel.currentIndex.row ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text: diameter
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                            Control {
                                id:diameterToolTipControl
                                anchors.fill: parent
                            }
                            ToolTip.visible: diameterToolTipControl.hovered
                            ToolTip.text: text
                        }

                        TableView.editDelegate: SpinBox {
                            anchors.margins: 1
                            anchors.fill: parent
                            value: diameter
                            editable:true
                            from:0
                            to:100
                            TableView.onCommit:tableView.model.setData(tableView.index(row, column), value, MeltTableModel.Diameter)
                        }
                    }
                }

                DelegateChoice {
                    column: 4

                    delegate:Rectangle {
                        color: row === tableView.selectionModel.currentIndex.row ? "#46607c" : (row %2==0?"#2b2b2b":"#282828")
                        CheckBox {
                            anchors.centerIn: parent
                            checked: descriptionAppeared
                            onClicked: {
                                let modelIndex = tableView.index(row, 0);
                                tableView.model.setData(modelIndex, checked, MeltTableModel.DescriptionAppeared);
                                tableView.selectionModel.setCurrentIndex(modelIndex, ItemSelectionModel.SelectCurrent)
                            }
                        }
                    }
                }
                DelegateChoice {
                    column: 5
                    delegate: Rectangle {
                        required property int row
                        color: row === tableView.selectionModel.currentIndex.row ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        ColorCarp{
                            id:colorPicker
                            anchors.fill: parent
                            anchors.margins: 3
                            color:dyestuff
                            onAccepted: tableView.model.setData(tableView.index(row, column), colorPicker.color, MeltTableModel.Dyestuff)
                        }
                    }
                }
            }
        }

    }
}
