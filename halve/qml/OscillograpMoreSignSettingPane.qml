import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import "models"
Item {
    id:root
    required property var profile
    required property var catheter
    required property var effectiveElectrodeNodeListModel
    property var electrodeList : profile.oscillograpElectrode

    implicitWidth: layout.width
    implicitHeight: layout.height
    ColumnLayout {
        id:layout
        ElectrodeModel{
            id:electrodeModel
            signal resetModel();
        }


        Repeater {
            id:electrodeModelRepeater
            model: electrodeModel
            delegate: GroupBox {
                id:electrodeModelGroupBox
                required property var text
                required property var value
                required property int index

                function visibleState(){
                    switch(root.catheter.electrode) {
                    case Halve.BOTH: {
                        return value === Halve.SINGLE;
                    }
                    case Halve.SINGLE: {
                        return value === Halve.BOTH;
                    }
                    case Halve.PAIR: {
                        return value === Halve.SINGLE;
                    }
                    }
                   return true;
               }
                CatheterElectrodeNodeListModel{
                    id:catheterElectrodeNodeListModel
                    profile:root.profile
                    catheter:root.catheter
                    electrodeType:value
                }

                Connections {
                    target: electrodeModel
                    function onResetModel(){
                        catheterElectrodeNodeListModel.reset();
                    }
                }
                label: RowLayout {
                    width: parent.width
                    Label {
                        text:qsTr(electrodeModelGroupBox.text)
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    spacing:9
                    GridLayout {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        columns: 6
                        columnSpacing:9
                        Repeater {
                            model: catheterElectrodeNodeListModel
                            delegate: RowLayout {
                                required property int index
                                required property var name
                                required property var id
                                CheckBox {
                                    text:name
                                    checked: electrodeList.exist(id)
                                    onClicked: {
                                        if (checked) {
                                            electrodeList.add(id)
                                        } else {
                                            electrodeList.remove(id)
                                        }
                                        effectiveElectrodeNodeListModel.reset();
                                        electrodeModel.resetModel();
                                    }
                                    enabled: electrodeList.indexOfName(name) === -1 &&  effectiveElectrodeNodeListModel.indexOfName(name) === -1
                                }
                            }
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                }


            }
            Layout.fillWidth: true
        }
    }

}
