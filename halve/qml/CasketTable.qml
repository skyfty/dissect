
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve

Item {
    id:root

    required property var profile
    required property var casketListModel

    CatheterListModel{
        id:catheterListModel
        profile:root.profile
    }
    CatheterCasketFilterProxyModel {
        id:catheterFilterModel
        profile:root.profile
        sourceModel: catheterListModel
    }

    CatheterTypeModel {
        id:catheterTypeModel
        catheterTypeDb:ProfileManager.catheterRepertory.catheterTypeDb
    }

    ColumnLayout {

        anchors.centerIn: parent
        anchors.fill: parent
        spacing: 10

        Component {
             id: highlight
             Rectangle {
                 width: grid.cellWidth; height: grid.cellHeight
                 color: "lightsteelblue"; radius: 5
                 x: grid.currentItem.x
                 y: grid.currentItem.y
                 Behavior on x { SpringAnimation { spring: 3; damping: 0.2 } }
                 Behavior on y { SpringAnimation { spring: 3; damping: 0.2 } }
             }
         }
        ColumnLayout {
            Repeater {
                model: 2
                GroupBox {
                    id:groupBox
                    required property int index
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    label: Label {
                        Layout.preferredHeight: 20
                        Layout.fillWidth: true
                        font.pixelSize: 15
                        text:qsTr("Catheter case") + (index + 1)
                    }

                    GridView {
                        id: grid
                        anchors.fill: parent
                        cellWidth: 90
                        cellHeight: 90
                        clip: true
                        model: CasketFilterProxyModel{
                            box:groupBox.index
                            sourceModel: casketListModel
                        }
                        highlight: highlight
                        highlightFollowsCurrentItem: false
                        delegate: Item {
                            width: grid.cellWidth
                            height: grid.cellHeight
                            Column {
                                spacing:3
                                anchors.fill: parent
                                Rectangle {
                                    radius:90
                                    color: "yellow"
                                    width: 85
                                    height: 85
                                    Rectangle {
                                        anchors.centerIn: parent
                                        radius:parent.radius
                                        color: catheter !== ""?"lightgreen":"#8e8e8e"
                                        width: parent.width - 4
                                        height: parent.height - 4
                                        ColumnLayout {
                                            anchors.centerIn: parent
                                            Text {
                                                text: name;
                                                font.pixelSize: 15
                                                font.bold: true
                                                Layout.fillWidth: true
                                                horizontalAlignment:Text.AlignHCenter
                                                color:catheter !== ""?"green":"white"
                                            }
                                            Text {
                                                elide:Text.ElideRight
                                                font.pixelSize: 15
                                                font.bold: true
                                                visible: catheter
                                                text: formatText();
                                                Layout.fillWidth: true
                                                horizontalAlignment:Text.AlignHCenter
                                                color:catheter !== ""?"green":"white"

                                                function formatText() {
                                                    if (reserved) {
                                                        return catheter;
                                                    } else {
                                                        if (catheter) {
                                                            return catheterListModel.data(catheter, CatheterListModel.Type);
                                                        }
                                                    }
                                                    return "";
                                                }

                                            }
                                        }
                                    }
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    Menu {
                                        id: settingPopup
                                        x:parent.width / 2
                                        y: parent.height / 2
                                        contentItem:Pane {
                                            ColumnLayout{
                                                anchors.fill: parent
                                                ComboBox {
                                                    Layout.fillWidth: true
                                                    textRole: "type"
                                                    valueRole: "id"
                                                    currentIndex: -1
                                                    displayText: currentIndex === -1 ? qsTr("Please Choose") : currentText
                                                    model:catheterFilterModel
                                                    onActivated: {
                                                        casketListModel.occupy(idx, currentValue);
                                                        settingPopup.close();
                                                    }
                                                }
                                                Button {
                                                    text: qsTr("Reset")
                                                    Layout.fillWidth: true
                                                    onClicked: {
                                                        casketListModel.reset(idx);
                                                        settingPopup.close();
                                                    }
                                                }
                                            }
                                        }
                                        onAboutToShow: catheterFilterModel.box = idx
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        acceptedButtons: Qt.RightButton
                                        enabled: !reserved
                                        onClicked : {
                                            settingPopup.open()
                                        }
                                    }
                                }
                            }

                        }
                    }
                }
            }
        }
    }

}
