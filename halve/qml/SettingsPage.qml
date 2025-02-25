import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve 1.0
import Qt.labs.platform


Rectangle {
    id:root
    property alias visibleBackBtn:btnBack.visible

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal
        spacing: 0

        handle: Rectangle {
            implicitWidth: 4
            implicitHeight: parent.height
            color:"#303030"
        }
        Rectangle {
            color:"#303030"
            SplitView.fillHeight: true;
            SplitView.preferredWidth: 200
            SplitView.minimumWidth: 100
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10

                Rectangle {
                    color:"#545454"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius:10
                    Component {
                         id: highlight
                         Rectangle {
                             width: listView.width; height: 40
                             color: "lightsteelblue"; radius: 5
                             y: listView.currentItem.y
                             Behavior on y {
                                 SpringAnimation {
                                     spring: 3
                                     damping: 0.2
                                 }
                             }
                         }
                     }
                    ListView {
                        id:listView
                        anchors.fill: parent
                        flickableDirection: Flickable.AutoFlickIfNeeded
                        focus: true
                        model: ListModel {
                            ListElement {
                                name: qsTr("Interface")
                                file: "SettingInterfaceView.qml"
                            }
                            ListElement {
                                name: qsTr("Communication Settings")
                                file: "SettingCommunicationView.qml"
                            }
                            // ListElement {
                            //     name: qsTr("Shortcut key")
                            //     file: "SettingHotkey.qml"
                            // }
                        }
                        delegate: Rectangle  {
                            width: listView.width
                            height: 40
                            radius:5
                            color: ListView.isCurrentItem ? "#4772b3" : "#545454"

                            Item {
                                anchors.fill: parent
                                anchors.margins: 10
                                Label {
                                   anchors.verticalCenter: parent.verticalCenter
                                   text:name
                                   font.pixelSize: 14
                                   color:"white"
                               }
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    listView.currentIndex = index;
                                    stackView.replace(file)
                                }
                            }
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                }
                Button {
                    id:btnBack
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    text:qsTr("Back")
                    font.pixelSize: 17
                    font.bold: true
                    onClicked: {
                        root.StackView.view.pop()
                    }
                }                
            }
        }

        Rectangle {
            color:"#303030"
            SplitView.fillHeight: true;
            SplitView.fillWidth: true
            StackView {
                id: stackView
                anchors.fill: parent
                replaceEnter: Transition {
                     PropertyAnimation {
                         property: "opacity"
                         from: 0
                         to:1
                         duration: 200
                     }
                 }
                 replaceExit: Transition {
                     PropertyAnimation {
                         property: "opacity"
                         from: 1
                         to:0
                         duration: 200
                     }
                 }
                initialItem: SettingInterfaceView{}
            }
        }
    }

}
