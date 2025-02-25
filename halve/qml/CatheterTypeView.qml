import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtCharts 2.5
import QtQuick.Dialogs
import Qt.labs.qmlmodels 1.0
Page {
    id: root
    property var catheterTypeModel
    focusPolicy: Qt.ClickFocus
    header:ToolBar {
        id:toolbar
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        RowLayout {
            width: parent.width
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/back.png"
                    height: 25
                    width:25
                }
                onClicked:  {
                    root.StackView.view.pop();
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Back")
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/erase.png"
                    height: 25
                    width:25
                }
                onClicked:  {
                    tableView.closeEditor()
                    deleteConfirmDialog.open();
                }
                enabled: tableView.currentRow !== -1 && !catheterTypeModel.data(catheterTypeModel.index(tableView.currentRow,0), CatheterTypeModel.BUILDIN)
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Delete")


                ConfirmDialog {
                    id: deleteConfirmDialog
                    text: qsTr("Are you sure you want to delete this?")
                    onAccepted: {
                        catheterTypeModel.removeRows(tableView.currentRow, 1)
                    }
                }
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/add.png"
                    height: 25
                    width:25
                }
                onClicked:  {
                    tableView.closeEditor()
                    catheterTypeModel.insertRows(0, 1);
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Add")
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }

    Connections {
        target: tableView.model
        onRowsRemoved: {
            tableView.closeEditor()
            tableView.selectionModel.clearCurrentIndex();
        }
        onRowsInserted: {
            tableView.closeEditor()
            tableView.edit(tableView.index(0, 0))
        }
    }


    Rectangle {
        id: mainview
        color:"#333333"
        anchors.fill: parent
        ColumnLayout {
            id: tableViewLayout
            anchors.fill: parent
            spacing:0
            TableView {
                id: tableView
                alternatingRows:true
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.leftMargin: 5
                Layout.rightMargin: 5
                Layout.bottomMargin: 5
                columnWidthProvider: function () { return tableView.width / tableView.model.columnCount(); }
                rowHeightProvider:function(){return 30;}
                flickableDirection:Flickable.AutoFlickIfNeeded

                reuseItems: false
                clip: true
                model: catheterTypeModel

                selectionModel: ItemSelectionModel {}
                selectionMode: TableView.SingleSelection
                selectionBehavior: TableView.SelectRows
                delegate: Rectangle {
                    required property int row
                    color: row === tableView.selectionModel.currentIndex.row ? "#46607c" : (row %2==0?"#2b2b2b":"#282828")
                    Text {
                        anchors.fill: parent
                        text: name
                        font.pixelSize: 15
                        anchors.margins: 5
                        elide:Text.ElideRight
                        color:"white"
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment:Text.AlignVCenter
                        Control {
                            id:nameToolTipControl
                            anchors.fill: parent
                        }
                        ToolTip.visible: nameToolTipControl.hovered
                        ToolTip.text: text
                    }
                    TableView.editDelegate: TextField {
                        anchors.fill: parent
                        text: name
                        font.pixelSize: 15
                        horizontalAlignment: TextInput.AlignLeft
                        verticalAlignment: TextInput.AlignVCenter
                        Component.onDestruction: {
                            tableView.model.setData(tableView.index(row, column), text.trim(), CatheterTypeModel.NAME)
                        }
                    }
                }
            }
        }
    }
}
