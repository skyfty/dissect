
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Halve
import Qt.labs.qmlmodels
import QtQuick.Dialogs
import "models"

Rectangle {
    id: mainview
    required property var catheterRepertoryTableModel
    required property var catheterTypeModel

    property alias currentRow:tableView.currentRow
    signal mix(var row);
    color:"black"

    function setMagnetismOptions(index, consult, consultDistance, target, targetDistance) {
        tableView.model.setData(index, consult, CatheterTableModel.MagnetismConsult)
        tableView.model.setData(index, consultDistance, CatheterTableModel.MagnetismConsultDistance)
        tableView.model.setData(index, target, CatheterTableModel.MagnetismTarget)
        tableView.model.setData(index, targetDistance, CatheterTableModel.MagnetismTargetDistance)
    }

    ColumnLayout {
        id: tableViewLayout
        anchors.fill: parent
        spacing:0

        HorizontalHeaderView {
            id: headerView
            readonly property var columnNames: [
                qsTr("Enable"),
                qsTr("Amount"),
                qsTr("Electrode Type"),
                qsTr("Manufacturer"),
                qsTr("Brand"),
                qsTr("Type"),
                qsTr("Model"),
                qsTr("Gap"),
                qsTr("Fr"),
                qsTr("Diameter (mm)"),
                qsTr("Distal length"),
                qsTr("Electrode length"),
                qsTr("Magnetic"),
                qsTr("Color")]
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
                    horizontalAlignment: index === 0 || index === 13 || index === 12?Text.AlignHCenter:Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
        ElectrodeModel{
            id:electrodeModel
        }

        Connections {
            target: tableView.model
            onRowsRemoved: {
                tableView.closeEditor()
                tableView.selectionModel.clearCurrentIndex();
            }

            onRowsInserted: {
                tableView.closeEditor()
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

            selectionModel: ItemSelectionModel {}
            selectionMode: TableView.SingleSelection
            selectionBehavior: TableView.SelectRows

            reuseItems: false
            clip: true
            model: catheterRepertoryTableModel

            delegate: DelegateChooser {
                id: chooser
                DelegateChoice  {
                    column: 0
                    delegate:Rectangle {
                        color: row === tableView.currentRow ? "#46607c" : (row %2==0?"#2b2b2b":"#282828")
                        ToolButton {
                            anchors.centerIn: parent
                            icon {
                                source: "qrc:/assets/images/add2.svg"
                                height: 19
                                width:19
                            }
                            onClicked:  {
                                mix(row);
                            }
                            enabled: amount > 0
                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Add")
                        }
                    }
                }
                DelegateChoice {
                    column: 1
                    delegate:Rectangle {
                        color: row === tableView.currentRow ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text: amount
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                        }

                        TableView.editDelegate: SpinBox {
                            id:amountSpinBox
                            visible:  !prefab
                            anchors.margins: 1
                            anchors.fill: parent
                            value: amount
                            focus: true
                            from:0
                            to:48
                            editable:true
                            validator: IntValidator {
                                locale: amountSpinBox.locale.name
                                bottom: Math.min(amountSpinBox.from, amountSpinBox.to)
                                top: Math.max(amountSpinBox.from, amountSpinBox.to)
                            }
                            stepSize: 2
                            font.pixelSize: 15
                            Component.onDestruction: {
                                if (value % 2 != 0) {
                                    return;
                                }
                                tableView.model.setData(tableView.index(row, column), value, CatheterTableModel.Amount)
                                mainview.setMagnetismOptions(tableView.index(row, column),-1,0,-1,0);
                            }
                        }
                    }
                }
                DelegateChoice {
                    column: 2

                    delegate:Rectangle {
                        color: row === tableView.currentRow? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            text:formatText(electrode)
                            function formatText(v) {
                                return electrodeModel.getTextByValue(v)
                            }
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter

                            Control {
                                id:electrodeToolTipControl
                                anchors.fill: parent
                            }
                            ToolTip.visible:text !=="" &&  electrodeToolTipControl.hovered
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
                    column: 3

                    delegate:Rectangle {
                        color: row === tableView.currentRow ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            text: manufacturer
                            elide:Text.ElideRight
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                            Control {
                                id:manufacturerToolTipControl
                                anchors.fill: parent
                            }
                            ToolTip.visible:text !=="" &&  manufacturerToolTipControl.hovered
                            ToolTip.text: text
                        }

                        TableView.editDelegate: TextField {
                            anchors.margins: 1
                            anchors.fill: parent
                            text: manufacturer
                            visible:  !prefab
                            font.pixelSize: 15
                            horizontalAlignment: TextInput.AlignLeft
                            verticalAlignment: TextInput.AlignVCenter
                            Component.onDestruction:  tableView.model.setData(tableView.index(row, column), text, CatheterTableModel.Manufacturer)

                        }
                    }
                }

                DelegateChoice {
                    column: 4

                    delegate:Rectangle {
                        color: row === tableView.currentRow? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            text: name
                            elide:Text.ElideRight
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                            Control {
                                id:nameToolTipControl
                                anchors.fill: parent
                            }
                            ToolTip.visible:text !=="" &&  nameToolTipControl.hovered
                            ToolTip.text: text
                        }

                        TableView.editDelegate: TextField {
                            anchors.margins: 1
                            anchors.fill: parent
                            text: name
                            visible:  !prefab
                            font.pixelSize: 15
                            horizontalAlignment: TextInput.AlignLeft
                            verticalAlignment: TextInput.AlignVCenter
                            Component.onDestruction:  tableView.model.setData(tableView.index(row, column), text, CatheterTableModel.Name)

                        }
                    }
                }
                DelegateChoice {
                    column: 5

                    delegate:Rectangle {
                        color: row === tableView.currentRow ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
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
                            ToolTip.visible:text !=="" &&  typeToolTipControl.hovered
                            ToolTip.text: text
                        }
                        TableView.editDelegate: ComboBox {
                            anchors.margins: 1
                            textRole: "name"
                            valueRole: "name"
                            anchors.fill: parent
                            onActivated:tableView.model.setData(tableView.index(row, column), currentValue, CatheterTableModel.Type)
                            Component.onCompleted:currentIndex = indexOfValue(type)
                            model:catheterTypeModel
                        }
                    }
                }
                DelegateChoice {
                    column: 6
                    delegate:Rectangle {
                        color: row === tableView.currentRow ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text: version
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                            Control {
                                id:versionToolTipControl
                                anchors.fill: parent
                            }
                            ToolTip.visible:text !=="" &&  versionToolTipControl.hovered
                            ToolTip.text: text
                        }

                        TableView.editDelegate: TextField {
                            anchors.margins: 1
                            anchors.fill: parent
                            text: version
                            visible:  !prefab
                            font.pixelSize: 15
                            horizontalAlignment: TextInput.AlignLeft
                            verticalAlignment: TextInput.AlignVCenter
                            Component.onDestruction:  tableView.model.setData(tableView.index(row, column), text, CatheterTableModel.Version)

                        }
                    }
                }
                DelegateChoice {
                    column: 7
                    delegate:Rectangle {
                        color: row === tableView.currentRow ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            id:gapText
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text: gap.join('-')
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter

                            Control {
                                id:gapToolTipControl
                                anchors.fill: parent
                            }
                            ToolTip.visible:text !=="" &&  gapToolTipControl.hovered
                            ToolTip.text: text
                        }

                        TableView.editDelegate: TextField {
                            anchors.margins: 1
                            anchors.fill: parent
                            text: gapText.text
                            visible:  !prefab
                            font.pixelSize: 15
                            horizontalAlignment: TextInput.AlignLeft
                            verticalAlignment: TextInput.AlignVCenter
                            Component.onDestruction: {
                                var regex = /^[0-9-]+$/;
                                if (regex.test(text)) {
                                    var val = text.split("-").filter(item => item.trim() !== "");
                                    if (val.length === (amount -1)) {
                                        tableView.model.setData(tableView.index(row, column), val, CatheterTableModel.Gap)
                                    }

                                }
                            }
                        }
                    }

                }
                DelegateChoice {
                    column: 8
                    delegate:Rectangle {
                        color: row === tableView.currentRow? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text: fr
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                        }

                        TableView.editDelegate: TextField {
                            anchors.margins: 1
                            anchors.fill: parent
                            text: fr
                            visible:  !prefab
                            font.pixelSize: 15
                            horizontalAlignment: TextInput.AlignLeft
                            verticalAlignment: TextInput.AlignVCenter
                            Component.onDestruction:  tableView.model.setData(tableView.index(row, column), text, CatheterTableModel.Fr)

                        }
                    }

                }
                DelegateChoice {
                    column: 9
                    delegate:Rectangle {
                        color: row === tableView.currentRow ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text: diameter
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                        }

                        TableView.editDelegate: TextField {
                            anchors.margins: 1
                            anchors.fill: parent
                            text: diameter
                            visible:  !prefab
                            validator: DoubleValidator {}
                            font.pixelSize: 15
                            horizontalAlignment: TextInput.AlignLeft
                            verticalAlignment: TextInput.AlignVCenter
                            Component.onDestruction:  tableView.model.setData(tableView.index(row, column), text, CatheterTableModel.Diameter)

                        }
                    }
                }
                DelegateChoice {
                    column: 10
                    delegate:Rectangle {
                        color: row ===tableView.currentRow? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text: far
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                        }

                        TableView.editDelegate: TextField {
                            anchors.margins: 1
                            anchors.fill: parent
                            text: far
                            visible:  !prefab
                            font.pixelSize: 15
                            validator: DoubleValidator {}
                            horizontalAlignment: TextInput.AlignLeft
                            verticalAlignment: TextInput.AlignVCenter
                            Component.onDestruction:  tableView.model.setData(tableView.index(row, column), text, CatheterTableModel.Far)

                        }
                    }
                }

                DelegateChoice {
                    column: 11
                    delegate:Rectangle {
                        color: row ===tableView.currentRow? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text: electrodeLength
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                        }

                        TableView.editDelegate: TextField {
                            anchors.margins: 1
                            anchors.fill: parent
                            text: electrodeLength
                            visible:  !prefab
                            font.pixelSize: 15
                            validator: DoubleValidator {}
                            horizontalAlignment: TextInput.AlignLeft
                            verticalAlignment: TextInput.AlignVCenter
                            Component.onDestruction:  tableView.model.setData(tableView.index(row, column), text, CatheterTableModel.ElectrodeLength)

                        }
                    }
                }
                DelegateChoice {
                    column: 12
                    delegate:Rectangle {
                        color: row === tableView.currentRow? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        RowLayout {
                            anchors.margins: 1
                            anchors.fill: parent
                            CheckBox {
                               enabled: !prefab
                               checked: magnetism
                               onClicked: {
                                   tableView.model.setData(tableView.index(row, column), checked, CatheterTableModel.Magnetism)
                               }
                            }

                            Label {
                                text: (magnetismConsult === -1 || magnetismTarget === -1)?qsTr("Select port"): magnetismDescription
                                visible: magnetism && amount > 0
                                ToolTip.visible:text !==""
                                ToolTip.text: text

                                TapHandler {
                                    onDoubleTapped: {
                                        magnetismOptionsPopup.open()
                                    }
                                    enabled: magnetism && amount > 0
                                }
                                Menu {
                                    id: magnetismOptionsPopup
                                    y: 0
                                    contentItem: Pane {
                                        focusPolicy: Qt.ClickFocus
                                        RowLayout {
                                            anchors.fill: parent
                                            MagnetismOptionsPane {
                                                id:magnetismOptionsPane
                                                magnetismAmount:amount
                                                consult:magnetismConsult
                                                consultDistance:magnetismConsultDistance
                                                target:magnetismTarget
                                                targetDistance:magnetismTargetDistance
                                            }
                                        }
                                    }
                                    onAboutToHide: {
                                        if (magnetismOptionsPane.consult === magnetismOptionsPane.target) {
                                            return;
                                        }
                                        mainview.setMagnetismOptions(tableView.index(row, column),
                                                            magnetismOptionsPane.consult,
                                                            magnetismOptionsPane.consultDistance,
                                                            magnetismOptionsPane.target,
                                                            magnetismOptionsPane.targetDistance);
                                    }
                                }
                            }
                        }


                    }
                }
                DelegateChoice {
                    column: 13
                    delegate: Rectangle {
                        required property int row
                        color: row === tableView.currentRow ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        ColorCarp{
                            id:colorPicker
                            anchors.fill: parent
                            anchors.margins: 3
                            color:dyestuff
                            onAccepted: tableView.model.setData(tableView.index(row, column), colorPicker.color, CatheterTableModel.Dyestuff)
                        }
                    }
                }
            }
        }

    }
}
