
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels
import Qt.labs.platform
import "models"

Rectangle {
    id: mainview
    property alias model:tableView.model
    property alias count:tableView.rows
    required property var catheterTypeModel
    required property var channelMode
    property var revisable
    property alias currentRow:tableView.currentRow
    color:"#333333"

    function index(row) {
        return tableView.index(row, 0);
    }

    ColumnLayout {
        id: tableViewLayout
        anchors.fill: parent
        spacing:0

        HorizontalHeaderView {
            id: headerView
            readonly property var columnNames: [qsTr("Type"),qsTr("Port"),qsTr("Active"),qsTr("Apparent"),qsTr("Electrode Type"),qsTr("Color")]
            syncView: tableView
            clip: true
            Layout.fillWidth: true
            flickableDirection:Flickable.AutoFlickIfNeeded
            resizableColumns: false // otherwise QTBUG-111013 happens
            delegate: Rectangle{
                required property int index
                color:"#333333"
                implicitHeight: 30
                implicitWidth: 100
                Text {
                    color:"#e7e7e7"
                    font.pixelSize: 14
                    anchors.fill: parent
                    font.bold: true
                    anchors.leftMargin: 4
                    text: headerView.columnNames[index]?headerView.columnNames[index]:""
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        ElectrodeModel{
            id:electrodeModel
        }

        CatheterPortModel{
            id:catheterPortModel
        }
        Connections {
            target: tableView.model
            function onRowsRemoved() {
                tableView.selectionModel.clearCurrentIndex();
            }
        }

        TableView {
            id: tableView
            Layout.fillHeight: true
            Layout.fillWidth: true
            alternatingRows:true
            columnWidthProvider: function (column) {
                return (tableView.width) / (headerView.columnNames.length);
            }
            rowHeightProvider:function(){return 30;}
            flickableDirection:Flickable.AutoFlickIfNeeded

            selectionModel: ItemSelectionModel {}
            selectionMode: TableView.SingleSelection
            selectionBehavior: TableView.SelectRows
            reuseItems: false
            clip: true
            editTriggers: mainview.revisable?(TableView.DoubleTapped | TableView.EditKeyPressed):TableView.NoEditTriggers

            delegate: DelegateChooser {
                id: chooser
                DelegateChoice  {
                    column: 0
                    delegate:Rectangle {
                        enabled: mainview.revisable
                        color: row ===  tableView.currentRow ? "#46607c" : (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text:type
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                            Control {
                                id:typeToolTipControl
                                anchors.fill: parent
                            }
                            ToolTip.visible:text !=="" && typeToolTipControl.hovered
                            ToolTip.text: text
                        }
                        TableView.editDelegate: ComboBox {
                            anchors.margins: 1
                            textRole: "name"
                            valueRole: "name"
                            anchors.fill: parent
                            model:catheterTypeModel
                            onActivated:tableView.model.setData(tableView.index(row, column), currentValue, CatheterTableModel.Type)
                            Component.onCompleted:currentIndex = indexOfValue(type)
                        }
                    }
                }

                DelegateChoice  {
                    column: 1
                    delegate:Rectangle {
                        enabled: mainview.revisable && magnetism
                        color: row ===  tableView.currentRow ? "#46607c" : (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text:magnetism?catheterPortModel.getTextByValue(port):qsTr("Invalid")
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                            Control {
                                id:portToolTipControl
                                anchors.fill: parent
                            }
                            ToolTip.visible:text !=="" && portToolTipControl.hovered
                            ToolTip.text: text
                        }
                        TableView.editDelegate: ComboBox {
                            id:portComboBox
                            anchors.margins: 1
                            anchors.fill: parent
                            textRole: "text"
                            valueRole: "value"
                            model: catheterPortModel

                            delegate: RowLayout {
                                id:portWarp
                                required property var index
                                required property var enabled
                                required property var text
                                required property var value
                                Button {
                                    id:portButton
                                    implicitWidth: portComboBox.width
                                    enabled: portWarp.enabled
                                    contentItem: Label {
                                        text: portWarp.text
                                        font.weight: portComboBox.currentIndex === index ? Font.Black : Font.Normal
                                        color: portWarp.enabled?"white":"#626264"
                                    }
                                    background: Rectangle {
                                        color: portButton.down ? "#31609b":portWarp.enabled && portButton.hovered?"#6493ce":"#404040";
                                        visible: portButton.down  || portButton.highlighted  || (portWarp.enabled && portButton.hovered)
                                    }
                                    onClicked: {
                                        tableView.model.setData(tableView.index(row, column), value, CatheterTableModel.Port)
                                        tableView.closeEditor();
                                    }

                                    ToolTip.visible:portButton.hovered
                                    ToolTip.text: portWarp.text

                                }
                            }
                            visible: magnetism
                            Component.onCompleted:currentIndex = indexOfValue(port)
                        }
                    }
                }
                DelegateChoice  {
                    column: 2
                    delegate:Rectangle {
                        enabled: mainview.revisable
                        color: row ===  tableView.currentRow ? "#46607c" : (row %2==0?"#2b2b2b":"#282828")
                        RadioButton {
                            anchors.centerIn: parent
                            checked: active
                            enabled: (magnetism || mainview.channelMode === Halve.CHANNELMODE_ELECTRICAL) && employ
                            onClicked: {
                                let modelIndex = tableView.index(row, 0);
                                tableView.model.setData(modelIndex, !checked, CatheterTableModel.ACTIVE);
                                tableView.selectionModel.setCurrentIndex(modelIndex, ItemSelectionModel.SelectCurrent)
                            }
                            hoverEnabled: true
                            ToolTip.visible: hovered && magnetism
                            ToolTip.text: qsTr("Set up the current catheter modelling and mapping")
                        }
                    }
                }
                DelegateChoice  {
                    column: 3
                    delegate:Rectangle {
                        enabled: mainview.revisable
                        color: row ===  tableView.currentRow ? "#46607c" : (row %2==0?"#2b2b2b":"#282828")
                        ApparentSwitch {
                            anchors.centerIn: parent
                            checked: apparent
                            onClicked: {
                                let modelIndex = tableView.index(row, 0);
                                tableView.model.setData(modelIndex, checked, CatheterTableModel.APPARENT);
                                tableView.selectionModel.setCurrentIndex(modelIndex, ItemSelectionModel.SelectCurrent)
                            }
                        }
                    }
                }
                DelegateChoice {
                    column: 4

                    delegate:Rectangle {
                        enabled: mainview.revisable
                        color: row ===  tableView.currentRow ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text:formatText(electrode)
                            function formatText(v) {
                                return electrodeModel.getTextByValue(v)
                            }
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter

                            Control {
                                id:toolTipControl
                                anchors.fill: parent
                            }
                            ToolTip.visible:text !=="" &&  toolTipControl.hovered
                            ToolTip.text: text
                        }
                        TableView.editDelegate: ComboBox {
                            anchors.margins: 1
                            textRole: "text"
                            valueRole: "value"
                            anchors.fill: parent
                            onActivated:tableView.model.setData(tableView.index(row, column), currentValue, CatheterTableModel.Electrode)
                            Component.onCompleted:currentIndex = indexOfValue(electrode)
                            model:electrodeModel
                        }
                    }
                }

                DelegateChoice {
                    column: 5
                    delegate: Rectangle {
                        enabled: mainview.revisable
                        color: row ===  tableView.currentRow ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        ColorCarp{
                            id:colorPicker
                            anchors.fill: parent
                            anchors.margins: 3
                            color:dyestuff
                            onChanged: {
                                tableView.model.setData(tableView.index(row, column), colorPicker.color, CatheterTableModel.Dyestuff)
                            }
                        }
                    }
                }
            }
        }

    }
}
