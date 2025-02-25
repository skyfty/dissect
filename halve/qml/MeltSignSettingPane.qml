import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts

 ColumnLayout {
     id:root
     required property var profile
     property var consultElectrodeId
     required property var catheterListModel
     required property var electrodeList
     required property var electrodeListFilter
     property alias bodyVisible: bodySurfaceGroupBox.visible

    GroupBox {
        id:bodySurfaceGroupBox
        ButtonGroup {
            id:bodySurfaceGroupBoxGroup
            exclusive:false
        }
        label: RowLayout {
            width: parent.width
            Label {
                text:"ECG"
            }
            Item {
                Layout.fillWidth: true
            }

            CheckBox {
                text:"全选"
                checkState :bodySurfaceGroupBoxGroup.checkState
                onClicked: {
                    for(var i = 0; i < bodySurfaceElectrodeNodeListModel.rowCount(); ++i) {
                        var item = bodySurfaceElectrodeNodeListModel.get(i)
                        if (electrodeListFilter.exist(item) || item.id === root.consultElectrodeId) {
                            continue;
                        }
                        if (checked) {
                            electrodeList.add(item)
                        } else  {
                            electrodeList.remove(item)
                        }
                    }
                    bodySurfaceElectrodeNodeListModel.reset();
                }
            }
        }

        Layout.fillWidth: true
        GridLayout {
            anchors.fill: parent
            columns: 4
            columnSpacing:9

            BodySurfaceElectrodeNodeListModel{
                id:bodySurfaceElectrodeNodeListModel
                profile:root.profile
            }
            MeltElectrodeNodeFilterModel {
                profile:root.profile
                id:bodySurfaceElectrodeNodeFilterListModel
                exposedElectrode:electrodeListFilter
                sourceModel: bodySurfaceElectrodeNodeListModel
                consultElectrodeId:root.consultElectrodeId
            }

            Repeater {
                model: bodySurfaceElectrodeNodeFilterListModel
                delegate: RowLayout {
                    required property int index
                    required property var name
                    required property var id
                    CheckBox {
                        text:name
                        ButtonGroup.group: bodySurfaceGroupBoxGroup
                        checked: electrodeList.exist(id)
                        onClicked: {
                            if (checked) {
                                electrodeList.add(id)
                            } else {
                                electrodeList.remove(id)
                            }
                        }
                    }
                }
            }
        }
    }

    Repeater {
        model: catheterListModel
        delegate: GroupBox {
            required property var name
            required property int index
            property var currentCatheter:catheterListModel.get(index)
            visible: currentCatheter !== null && currentCatheter.employ
            CatheterElectrodeNodeListModel{
                id:catheterElectrodeNodeListModel
                profile:root.profile
                catheter:currentCatheter
            }

            ButtonGroup {
                id:catheterElectrodeNodeGroupBoxGroup
                exclusive:false
            }
            MeltElectrodeNodeFilterModel {
                profile:root.profile
                id:catheterElectrodeNodeFilterListModel
                exposedElectrode:electrodeListFilter
                sourceModel: catheterElectrodeNodeListModel
                consultElectrodeId:root.consultElectrodeId
            }

            label: RowLayout {
                width: parent.width
                Label {
                    text:currentCatheter !== null &&currentCatheter.type
                }
                Item {
                    Layout.fillWidth: true
                }

                CheckBox {
                    text:"全选"
                    checkState :catheterElectrodeNodeGroupBoxGroup.checkState
                    onClicked: {
                        for(var i = 0; i < catheterElectrodeNodeListModel.rowCount(); ++i) {
                            var item = catheterElectrodeNodeListModel.get(i)
                            if (electrodeListFilter.exist(item)) {
                                continue;
                            }

                            if (checked) {
                                electrodeList.add(item)
                            } else {
                                electrodeList.remove(item)
                            }
                        }
                        catheterElectrodeNodeListModel.reset();
                    }
                }
            }
            Layout.fillWidth: true
            RowLayout {
                anchors.fill: parent
                spacing:9
                GridLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    columns: 4
                    columnSpacing:9
                    Repeater {
                        model: catheterElectrodeNodeFilterListModel
                        delegate: RowLayout {
                            required property int index
                            required property var name
                            required property var id
                            CheckBox {
                                text:name
                                ButtonGroup.group: catheterElectrodeNodeGroupBoxGroup
                                checked: electrodeList.exist(id)
                                onClicked: {
                                    if (checked) {
                                        electrodeList.add(id)
                                    } else {
                                        electrodeList.remove(id)
                                    }
                                    catheterElectrodeNodeListModel.reset();
                                }
                            }
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                }
            }
        }
    }
 }

