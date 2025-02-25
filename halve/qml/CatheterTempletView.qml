import QtQuick
import Qt.labs.platform  as Platform
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels

ColumnLayout {
    id:root
    property alias model:tableView.model
    property alias currentRow:tableView.currentRow

    signal accepted();
    signal close();

    RowLayout {
        Layout.fillWidth: true

        ToolButton {
            Layout.alignment: Qt.AlignVCenter
            icon {
                source: "qrc:/assets/images/erase.png"
                height: 19
                width:19
            }
            onClicked:  {
                tableView.closeEditor()
                deleteConfirmDialog.open();
            }
            enabled: tableView.currentRow !== -1
            hoverEnabled: true
            ToolTip.visible: hovered
            ToolTip.text: qsTr("Delete")

            ConfirmDialog {
                id: deleteConfirmDialog
                text: qsTr("Are you sure you want to delete this?")
                onAccepted: {
                    root.model.removeRows(tableView.currentRow, 1)
                }
            }
        }
        Item {
            Layout.fillWidth: true
        }

        SearchInput {
            id:searchInput
            Layout.fillHeight: true
            Layout.minimumWidth:150
            Layout.margins: 1
            Layout.maximumHeight: 30
            model: root.model
        }
    }
    Connections {
        target: tableView.model
        onRowsRemoved: {
            tableView.closeEditor()
            tableView.selectionModel.clearCurrentIndex();
        }
    }


    Rectangle {
        id: userList
        color:"#333333"
        Layout.fillHeight: true
        Layout.fillWidth: true

        TableView {
            id: tableView
            alternatingRows:true
            anchors.fill: parent
            rowHeightProvider:function(){return 30;}
            flickableDirection:Flickable.AutoFlickIfNeeded
            reuseItems: false
            clip: true
            columnWidthProvider: function () { return tableView.width / tableView.model.columnCount(); }

            selectionModel: ItemSelectionModel {}
            selectionMode: TableView.SingleSelection
            selectionBehavior: TableView.SelectRows

            delegate: Rectangle {
                required property int row
                color: row === tableView.currentRow ? "#46607c" : (row %2==0?"#2b2b2b":"#282828")
                Text {
                    anchors.fill: parent
                    anchors.margins: 5
                    elide:Text.ElideRight
                    text: name
                    font.pixelSize: 15
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
                        tableView.model.setData(tableView.index(row, column), text.trim(), CatheterTempletListModel.NAME)
                    }
                }

            }
        }
    }

    RowLayout {
        Layout.fillWidth: true
        Button {
            id:okButton
            Layout.fillWidth: true
            text: qsTr("Import")
            implicitHeight: 20
            enabled: tableView.currentRow !== -1
            onClicked: {
                root.accepted();
            }
            background: Rectangle {
                implicitWidth: 80
                implicitHeight: 24
                radius: 2
                color: okButton.down ? "#31609b": "#6493ce";
            }
        }

        Button {
            id:noButton
            Layout.fillWidth: true
            text: qsTr("Cancel")
            implicitHeight: 20
            onClicked: {
                root.close();
            }
            background: Rectangle {
                implicitWidth: 80
                implicitHeight: 24
                radius: 2
                color: noButton.down ? "#31609b": noButton.hovered?"#6493ce":"#404040";
            }
        }
    }
}
