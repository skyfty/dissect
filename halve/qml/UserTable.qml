
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
    property alias currentRow:tableView.currentRow
    color:"black"

    signal doubleClick();

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
                qsTr("Identify"),
                qsTr("Name"),
                qsTr("Sex"),
                qsTr("Birthday"),
                qsTr("Weight"),
                qsTr("Doctor"),
                qsTr("Number"),]
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
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
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
            }
        }


        TableView {
            id: tableView
            alternatingRows:true
            Layout.fillHeight: true
            Layout.fillWidth: true
            columnWidthProvider: function () { return tableView.width / 7; }
            rowHeightProvider:function(){return 30;}
            flickableDirection:Flickable.AutoFlickIfNeeded

            selectionModel: ItemSelectionModel {}
            selectionMode: TableView.SingleSelection
            selectionBehavior: TableView.SelectRows

            reuseItems: false
            clip: true

            delegate: DelegateChooser {
                id: chooser

                DelegateChoice {
                    column: 0

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

                        TapHandler {
                            onDoubleTapped:mainview.doubleClick();
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
                            text: name
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                        }

                        TapHandler {
                            onDoubleTapped:mainview.doubleClick();
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

                        TapHandler {
                            onDoubleTapped:mainview.doubleClick();
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
                            elide:Text.ElideRight
                            text: birthday.toLocaleDateString(InterfaceSettings.language)
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                        }

                        TapHandler {
                            onDoubleTapped:mainview.doubleClick();
                        }
                    }
                }

                DelegateChoice {
                    column: 4

                    delegate:Rectangle {
                        color: row === tableView.currentRow ? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text:weight +" Kg"
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                        }

                        TapHandler {
                            onDoubleTapped:mainview.doubleClick();
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
                            text: doctor
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                        }


                        TapHandler {
                            onDoubleTapped:mainview.doubleClick();
                        }
                    }
                }
                DelegateChoice {
                    column: 6

                    delegate:Rectangle {
                        color: row === tableView.currentRow? "#46607c" :  (row %2==0?"#2b2b2b":"#282828")
                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            elide:Text.ElideRight
                            text: id
                            font.pixelSize: 15
                            color:"white"
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment:Text.AlignVCenter
                        }


                        TapHandler {
                            onDoubleTapped:mainview.doubleClick();
                        }
                    }
                }
            }
        }

    }
}
