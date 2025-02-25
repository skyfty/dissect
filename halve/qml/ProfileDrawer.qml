import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import "models"

ColumnLayout {
    id:root
    property var currentUser: ProfileManager.currentUser
    required property var profileListModel
    signal newStudyClicked();
    signal open(var id);
    signal deleted(var index);
    signal about();

    Connections {
        target: ProfileManager
        function onCurrentProfileChanged(){
            listView.currentIndex = profileListModel.indexFromId(ProfileManager.currentProfile.id).row;
        }
    }
    Rectangle {
        Layout.preferredHeight:60
        Layout.fillWidth: true
        color:"black"
        RowLayout {
            anchors.fill: parent
            Rectangle {
                Layout.preferredWidth:60
                Layout.preferredHeight: 60
                color:"white"
                radius:90
                Image {
                    height:40
                    width:40
                    anchors.centerIn: parent
                    source: currentUser.sex===User.Man? "qrc:/assets/images/man.png": "qrc:/assets/images/woman.png"
                }
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 1
                    Label {
                        text:currentUser.name
                        font.pixelSize: 20
                        font.bold: true
                        Layout.fillWidth: true
                        elide:Text.ElideRight
                    }
                    Label {
                        text:currentUser.idcard
                        font.pixelSize: 13
                        font.bold: true
                    }
                    Item {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }
                }
            }
            ColumnLayout {
                Layout.margins: 0
                spacing:0
                ToolButton {
                    icon {
                        source: "qrc:/assets/images/about.png"
                        height: 20
                        width:20
                    }
                    onClicked:  {
                        root.about();
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("About Us")

                }

                ToolButton {
                    icon {
                        source: "qrc:/assets/images/help.png"
                        height: 20
                        width:20
                    }
                    onClicked:  {

                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Help")

                }
            }


        }
    }

    ChannelModeModel {
        id:channelModeModel
    }

    Component {
        id:openProfilePopupComponent
        Menu {
            id: openProfilePopup
            contentItem:Pane {
                focusPolicy: Qt.ClickFocus
                ColumnLayout {
                    anchors.fill: parent
                    MenuItem {
                        text:qsTr("Open")
                        enabled: root.profileListModel.indexFromId(ProfileManager.currentProfile.id).row !== listView.currentIndex
                        onTriggered: {
                            root.open(listView.currentIndex);
                            openProfilePopup.close()
                        }
                        icon {
                            source: "qrc:/assets/images/profile.png"
                        }
                    }
                    MenuItem {
                        text:qsTr("Show in explore")
                        onTriggered: {
                            var selectedId = root.profileListModel.data(root.profileListModel.index(listView.currentIndex, 0), ProfileListModel.Id)
                            root.profileListModel.showInFolder(selectedId);
                            openProfilePopup.close()
                        }
                        icon {
                            source: "qrc:/assets/images/showinfolder.svg"
                        }
                    }
                }
            }

            onAboutToHide: {
                openProfilePopup.destroy();
            }

        }
    }

    Rectangle {
        Layout.fillHeight: true
        Layout.fillWidth: true
        color: "#333333"
        radius:5

        Component {
            id: contactsDelegate
            Control {
                id: wrapper
                width: ListView.view.width
                height: 50
                Rectangle {
                    anchors.fill: parent
                    radius: 10
                    color: ProfileManager.currentProfile!==null && profileListModel.indexFromId(ProfileManager.currentProfile.id).row === index?"#1d314d": "transparent"
                    RowLayout {
                       anchors.margins: 5
                       anchors.fill: parent
                       ChannelModeImage {
                           id:channelModeImage
                           Layout.preferredHeight: 40
                           Layout.preferredWidth: 40
                           state: channelMode
                       }
                       ColumnLayout {
                          Layout.fillHeight: true
                          Layout.fillWidth: true

                          Text {
                              Layout.fillWidth: true
                              text: doctor
                              color:"white"
                              font.pixelSize: 17
                          }
                          Text {
                              Layout.fillWidth: true
                              text: surgicalTime.toLocaleDateString(Qt.locale(InterfaceSettings.language))
                              color:"white"
                              font.pixelSize: 12
                          }

                       }
                   }
                }
                ToolTip {
                     parent: channelModeImage
                     visible: hovered
                     text: channelModeModel.getTextByValue(channelMode)
                 }

                TapHandler{
                    acceptedButtons: Qt.RightButton | Qt.LeftButton
                    onTapped:function(eventPoint, button) {
                        listView.currentIndex = index;
                        if (button === Qt.RightButton) {
                            openProfilePopupComponent.createObject(parent,{x:eventPoint.position.x,y:eventPoint.position.y}).open();
                        }
                    }


                    onDoubleTapped:function(eventPoint, button) {
                        if (button === Qt.LeftButton) {
                            listView.currentIndex = index;
                            root.open(listView.currentIndex);
                        }
                    }

                }

            }
        }
        ListView {
            id: listView
            anchors.fill: parent
            anchors.topMargin: 5
            anchors.bottomMargin: 5
            focus: true
            clip: true
            spacing:4
            flickableDirection: Flickable.AutoFlickDirection
            model: profileListModel
            delegate: contactsDelegate
            highlight: Rectangle {
                width: ListView.view.width
                height: 50
                radius: 10
                color: "#46607c";
                y: listView.currentItem != null?listView.currentItem.y:0
                Behavior on y {
                    SpringAnimation {
                        spring: 2
                        damping: 0.2
                    }
                }
            }
            highlightFollowsCurrentItem:false
         }
    }
    Item {
        Layout.preferredHeight: 120
        Layout.fillWidth: true;
        ColumnLayout {
            anchors.fill: parent
            Button {
                text:qsTr("Start a new study")
                Layout.fillHeight: true
                Layout.fillWidth: true
                onClicked: newStudyClicked()
                icon {
                    source: "qrc:/assets/images/study.png"
                }
            }

            Button {
                id:openButton
                text:qsTr("Open")
                enabled: listView.currentIndex !== -1 && profileListModel.indexFromId(ProfileManager.currentProfile.id).row !== listView.currentIndex
                Layout.fillHeight: true
                Layout.fillWidth: true
                onClicked: {
                    root.open(listView.currentIndex);
                }
                icon {
                    source: "qrc:/assets/images/profile.png"
                }
            }
            Button {
                text:qsTr("Remove the selection")
                enabled: listView.currentIndex !== -1
                Layout.fillHeight: true
                Layout.fillWidth: true
                onClicked: {
                    deleteConfirmDialog.open();
                }
                icon {
                    source: "qrc:/assets/images/erase.png"
                }
            }
        }

        ConfirmDialog {
            id: deleteConfirmDialog
            text: qsTr("Are you sure you want to delete this profile?")
            onAccepted: {
                root.deleted(listView.currentIndex);
            }
        }

    }
}
