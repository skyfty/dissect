
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels 1.0
import QtQuick.Window

Item {
    id:root
    required property var profile
    property var currentCatheter:null

    CatheterRepertoryTableModel {
        id:catheterRepertoryTableModel
        catheterDb:ProfileManager.catheterRepertory.catheterDb
        catheterTypeDb:ProfileManager.catheterRepertory.catheterTypeDb
    }

    CatheterTypeModel {
        id:catheterTypeModel
        catheterTypeDb:ProfileManager.catheterRepertory.catheterTypeDb
    }

    Component {
        id:catheterMeshViewComponent
        CatheterMeshView {
            id:catheterMeshView
            catheterDb:ProfileManager.catheterRepertory.catheterDb
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem:  Page {
            focusPolicy: Qt.ClickFocus

            header:ToolBar {

                RowLayout {
                    anchors.fill: parent
                    spacing: 1

                    ToolButton {
                        icon {
                            source: "qrc:/assets/images/add.png"
                            height: 25
                            width:25
                        }
                        onClicked:  {
                            catheterRepertoryTableModel.insertRows(0, 1);
                        }
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Add")
                    }
                    ToolButton {
                        focusPolicy: Qt.ClickFocus
                        icon {
                            source: "qrc:/assets/images/delete.png"
                            height: 25
                            width:25
                        }
                        onClicked:  {
                            deleteConfirmDialog.open();
                        }
                        ConfirmDialog {
                            id: deleteConfirmDialog
                            text: qsTr("Are you sure you want to delete this catheter?")
                            onAccepted: {
                                catheterRepertoryTableModel.removeRows(catheterTable.currentRow, 1)
                            }
                        }
                        enabled: currentCatheter !== null && !currentCatheter.prefab
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Delete")
                    }
                    ToolButton {
                        Layout.alignment: Qt.AlignVCenter
                        icon {
                            source: "qrc:/assets/images/sort.svg"
                            height: 19
                            width:19
                        }
                        onClicked: stackView.push("CatheterTypeView.qml", {catheterTypeModel:catheterTypeModel});
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Category")

                    }
                    ToolButton {
                        Layout.alignment: Qt.AlignVCenter
                        icon {
                            source: "qrc:/assets/images/catheter_mesh.svg"
                            height: 19
                            width:19
                        }
                        onClicked: {
                            if (currentCatheter !== null) {
                                stackView.push(catheterMeshViewComponent, {catheter: currentCatheter});
                            }
                        }
                        enabled:currentCatheter !== null && currentCatheter.amount !==0
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Catheter electrode")

                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    RowLayout {
                        Layout.margins: 5
                        Layout.fillHeight: true
                        Label {
                            text:qsTr("Type")
                        }
                        ComboBox {
                            id:typeFilter
                            Layout.fillHeight: true
                            displayText:typeFilterMenu.itemAt(0).text
                            popup: Menu {
                                id:typeFilterMenu
                                width: typeFilter.width
                                topMargin: 6
                                bottomMargin: 6
                                padding: 1
                                height: Math.min(contentItem.implicitHeight + 2, typeFilter.Window.height - topMargin - bottomMargin)
                                MenuItem {
                                    text:qsTr("All")
                                    font.bold: typeFilter.displayText === text
                                    onTriggered: {
                                        typeFilter.displayText = text
                                        catheterRepertoryProxyModel.type = ""
                                    }
                                }
                                Repeater {
                                    model:catheterTypeModel
                                    MenuItem {
                                        text:name
                                        font.bold: typeFilter.displayText === text
                                        onTriggered: {
                                            catheterRepertoryProxyModel.type = typeFilter.displayText = name
                                        }
                                    }
                                }

                                onAboutToHide: {
                                    searchInput.search();
                                }
                            }
                        }
                    }

                    SearchInput {
                        id:searchInput
                        Layout.fillHeight: true
                        Layout.minimumWidth:250
                        Layout.margins: 5
                        model: catheterRepertoryProxyModel
                    }
                }
            }

            CatheterSortFilterProxyModel {
                id:catheterRepertoryProxyModel
                sourceModel:catheterRepertoryTableModel
            }

            SplitView {
                anchors.fill: parent
                orientation: Qt.Vertical

                CatheterRepertoryTable {
                    id:catheterTable
                    SplitView.fillWidth: true
                    SplitView.fillHeight: true
                    catheterRepertoryTableModel:catheterRepertoryProxyModel
                    catheterTypeModel:catheterTypeModel
                    onMix:(row)=> {
                        var catheter = catheterRepertoryProxyModel.getData(row);
                        if (catheter !== null) {

                            root.profile.catheterDb.mix(catheter)
                        }
                    }

                    onCurrentRowChanged: {
                        currentCatheter = catheterRepertoryProxyModel.getData(catheterTable.currentRow);
                    }
                }
            }

            Component.onDestruction: {
                ProfileManager.catheterRepertory.save()
            }
        }
    }


}

