import QtQuick

import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtCharts 2.5
import QtQuick.Dialogs
import Qt.labs.qmlmodels 1.0
import Qt.labs.platform  as Platform
import "models"

Page {
    id: root
    required property var catheter
    required property var catheterDb

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
                    source: "qrc:/assets/images/camera_reset.png"
                    height: 25
                    width:25
                }
                onClicked:  {
                    stage.resetCamera()
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Reset camera")
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/mesh.svg"
                    height: 25
                    width:25
                }
                onClicked:  {
                    catheterElectrodeNodePointDb.editMesh();

                }
                enabled:!catheter.prefab
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Edit")
            }

            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/reset.svg"
                    height: 25
                    width:25
                }
                onClicked:  {
                    resetMeshConfirmDialog.open();
                }
                ConfirmDialog {
                    id: resetMeshConfirmDialog
                    text: qsTr("Are you sure you want to reset to default?")
                    onAccepted: {
                        catheterElectrodeNodePointDb.resetMesh();
                    }
                }

                enabled:!catheter.prefab
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Reset mesh")
            }

            ToolSeparator {}

            ColorCarp{
                id:dyestuffColorPicker
                Layout.preferredHeight: 25
                Layout.alignment: Qt.AlignLeft
                color:root.catheter.dyestuff
                onChanged: {
                    root.catheter.dyestuff =  dyestuffColorPicker.color.toString();
                }

                onAccepted: {
                    root.catheterDb.save();
                }
                Layout.preferredWidth: 100
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }

    CatheterElectrodeNodePointDb {
        id:catheterElectrodeNodePointDb
        catheter:root.catheter
        catheterDb:root.catheterDb
    }

    CatheterElectrodeTypeModel {
        id:catheterElectrodeTypeModel
    }

    Rectangle {
        id: mainview
        color:"#333333"
        anchors.fill: parent
        ColumnLayout {
            id: tableViewLayout
            anchors.fill: parent
            spacing:0
            Rectangle {
                Layout.fillWidth: true
                Rectangle {
                    anchors.fill: parent
                    color:"black"
                    opacity: 0.5
                }
                color: "transparent"
            }
            CatheterStage {
                id: stage
                clip: true
                Layout.fillHeight: true
                Layout.fillWidth: true
                catheter:root.catheter
                onInitRender:stage.resetRender()
            }
        }

        CatheterElectrodeNodePointListModel {
            id:catheterElectrodeNodePointListModel
            pointDb:catheterElectrodeNodePointDb
        }

        Item {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 10
            width: 200
            ColumnLayout {
                anchors.fill: parent
                spacing:0

                HorizontalHeaderView {
                    id: headerView
                    readonly property var columnNames: [qsTr("Id"),qsTr("Type")]
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
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
                TableView {
                    id: tableView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    columnWidthProvider: function () {
                        return tableView.width / tableView.model.columnCount();
                    }
                    rowHeightProvider:function(){return 30;}
                    flickableDirection:Flickable.AutoFlickIfNeeded
                    clip: true
                    model: catheterElectrodeNodePointListModel

                    delegate: DelegateChooser {
                        DelegateChoice {
                            column: 0
                            delegate:Item {
                                Rectangle {
                                    anchors.fill: parent
                                    opacity: 0.8
                                    color: row === tableView.currentRow? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                                }

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
                            column: 1
                            delegate:Item {
                                Rectangle {
                                    anchors.fill: parent
                                    opacity: 0.8
                                    color: row === tableView.currentRow? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                                }
                                Text {
                                    anchors.fill: parent
                                    anchors.margins: 5
                                    elide:Text.ElideRight
                                    text: catheterElectrodeTypeModel.getTextByValue(type)
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
