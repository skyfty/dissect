
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels
import "utils.js" as Utils

Rectangle {
    id: root
    property alias model:treeView.model
    property bool active:true
    property var reseauListModel
    property var currentIndex: treeView.index(treeView.currentRow, 0)


    color:"#333333"

    ColumnLayout {
        anchors.fill: parent
        spacing:0

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
                    treeView.model.setData(itemIndex, nameVal, ReseauListModel.Name)

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
        Component {
            id:treeDialogComponent
            Menu {
                id:treeDialog
                required property var row
                contentItem:Pane {
                    focusPolicy: Qt.ClickFocus
                    clip: true
                    ComboBox {
                        id:reseauComboBox
                        anchors.fill: parent
                        textRole: "name"
                        valueRole: "id"
                        model: reseauListModel
                        onActivated: {
                            treeView.model.moveRow(treeView.index(row, 0), currentValue);
                            treeDialog.close()
                        }
                    }
                }
                onAboutToHide: {
                    treeDialog.destroy()
                }
            }
        }


        TableView  {
            id: treeView
            Layout.fillHeight: true
            Layout.fillWidth: true

            flickableDirection: Flickable.AutoFlickIfNeeded
            clip: true
            reuseItems:false
            alternatingRows:true
            selectionModel: ItemSelectionModel {}
            rowHeightProvider:function(){return 40;}
            columnWidthProvider: function() { return width }
            delegate: Item {
                property bool held: false
                required property int index
                required property string name
                required property bool inuse
                required property bool buildin
                required property var dyestuff
                Rectangle {
                    anchors.fill: parent
                    color: index === treeView.currentRow ? "#46607c" :  (index %2==0?"#2b2b2b":"#282828")
                }
                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 5
                    Row {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        spacing: 3
                        Image {
                            id:icon
                            source: inuse?"qrc:/assets/images/active_mesh.svg":"qrc:/assets/images/mesh.svg"
                            height: 16
                            width:16
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Label {
                            id:nameLabel
                            anchors.verticalCenter: parent.verticalCenter
                            width: parent.width - icon.width
                            text: name
                            verticalAlignment:Text.AlignVCenter
                            height: parent.height -5
                            color: "white"
                            MouseArea {
                                anchors.fill: parent
                                onDoubleClicked: {
                                    if (root.active) {
                                        editDialogComponent.createObject(nameLabel, {width:nameLabel.width, height:nameLabel.height, row:index,text:name }).open();
                                    }
                                }
                                onClicked: {
                                    treeView.selectionModel.setCurrentIndex(treeView.index(index, 0), ItemSelectionModel.SelectCurrent)
                                }
                            }
                        }
                    }

                    Row {
                        spacing: 8
                        Layout.fillHeight: true
                        Layout.rightMargin: 5

                        ColorCarp{
                            id:colorPicker
                            color:dyestuff
                            width: 60
                            height: parent.height - 8
                            enabled: root.active
                            anchors.verticalCenter: parent.verticalCenter
                            onChanged: {
                                treeView.model.setData(treeView.index(index, column), colorPicker.color, ReseauListModel.DYESTUFF)
                            }
                        }
                        ApparentSwitch {
                            id:apparentCheckbox
                            checked: apparent
                            enabled: root.active
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: {
                                treeView.model.setData(treeView.index(index, column), checked, ReseauListModel.APPARENT)
                            }
                        }
                    }

                }


            }
        }
    }
}
