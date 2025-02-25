import QtQuick
import Qt.labs.platform  as Platform
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import QtCore
import "utils.js" as Utils

ApplicationWindow {
    id: mainWindow
    width: 1000
    height: 800
    visible: !ProfileManager.studying
    title: qsTr("Halve")

    Settings {
         property alias x: mainWindow.x
         property alias y: mainWindow.y
         property alias width: mainWindow.width
         property alias height: mainWindow.height
    }
    onClosing: {
        close.accepted = false
        StartoverState.exit(0);
    }



    onVisibilityChanged: {
        if (visible) {
            StartoverState.raise(mainWindow);
        }
    }

    UserListModel {
        id:userListModel
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Page {
           id: root
           ColumnLayout{
               anchors.centerIn: parent
                Button {
                    text: qsTr("New cases")
                    icon {
                        source: "qrc:/assets/images/user_add.png"
                        height: 25
                        width:25
                    }
                    font.bold: true
                    font.pixelSize: 17
                    Layout.fillWidth: true
                    onClicked: {
                        root.StackView.view.push("UserCreatePage.qml",{model:userListModel});
                    }
                    implicitWidth: 350
                    implicitHeight: 50
                }

                 RowLayout  {
                    Button {
                        text: qsTr("The last case")
                        font.bold: true
                        font.pixelSize: 17
                        implicitWidth: 200
                        implicitHeight: 50
                        icon {
                            source: "qrc:/assets/images/user_last.png"
                            height: 25
                            width:25
                        }
                        enabled: ProfileManager.lastUserId !== ""
                        onClicked: {
                            if (ProfileManager.open(ProfileManager.lastUserId)) {
                                ProfileManager.startStudy()
                            }
                        }
                    }
                    Button {
                        text: qsTr("Case library")
                        font.bold: true
                        font.pixelSize: 17
                        implicitWidth: 200
                        implicitHeight: 50
                        icon {
                            source: "qrc:/assets/images/userlist.png"
                            height: 25
                            width:25
                        }
                        onClicked: {
                            root.StackView.view.push("UserListPage.qml",{model:userListModel});
                        }
                    }
                }
                 RowLayout  {
                    Button {
                        id: importButton
                        text: qsTr("Import cases")
                        font.bold: true
                        font.pixelSize: 17
                        implicitWidth: 200
                        implicitHeight: 50
                        icon {
                            source: "qrc:/assets/images/import.png"
                            height: 25
                            width:25
                        }
                        background: Item {
                            Rectangle {
                                anchors.fill: parent
                                radius: 5
                                color: importButton.down ? "#31609b":importButton.hovered?"#6493ce":"#404040";
                            }
                        }
                        Platform.FolderDialog {
                             id: fileImportDialog
                             title: qsTr("Please choose a case path")
                             onAccepted: {
                                 var fileUrl = new String(fileImportDialog.folder);
                                 var newUser = ProfileManager.loadUser(fileUrl.substring(8));
                                 if (newUser !== null && ProfileManager.open(newUser)) {
                                     ProfileManager.startStudy()
                                 } else {
                                     Utils.showMessage(mainWindow, qsTr("Invalid case path"));
                                 }
                             }
                         }

                        onClicked: {
                            fileImportDialog.open();
                        }
                    }

                    Button {
                        id: exportButton
                        text: qsTr("Export cases")
                        font.bold: true
                        font.pixelSize: 17
                        implicitWidth: 200
                        implicitHeight: 50
                        icon {
                            source: "qrc:/assets/images/export.png"
                            height: 25
                            width:25
                        }
                        onClicked: {
                            userExportSelectDialogComponent.createObject(mainWindow).open();
                        }

                        Component {
                            id:userExportSelectDialogComponent
                            UserExportSelectDialog {
                                id: exportDialog
                                model:userListModel
                                onSelected: function (path, userIds) {
                                    userExporter.exportUsersToPath(userIds, path);
                                }
                            }
                        }
                        enabled: !userExporter.isRunning

                        UserExporter {
                            id: userExporter
                            onFinished: {
                                Utils.showMessage(mainWindow, qsTr("Case data export was completed"));
                            }
                        }
                        background: Item {
                            Rectangle {
                                anchors.fill: parent
                                radius: 5
                                color: exportButton.down ? "#31609b":exportButton.hovered?"#6493ce":"#404040";
                            }
                            ProgressBar {
                                id:exportProgressBar
                                anchors.fill: parent
                                value: userExporter.progressValue
                                visible: userExporter.isRunning
                            }
                        }
                    }
                }

                 Button {
                     text: qsTr("Settings")
                     font.bold: true
                     font.pixelSize: 17
                     icon {
                         source: "qrc:/assets/images/settings.png"
                         height: 25
                         width:25
                     }
                     Layout.fillWidth: true
                     implicitWidth: 350

                     implicitHeight: 50
                     onClicked: {
                         root.StackView.view.push("SettingsPage.qml");
                     }
                 }
                 Button {
                     text: qsTr("Exit")
                     font.bold: true
                     font.pixelSize: 17
                     icon {
                         source: "qrc:/assets/images/quit.png"
                         height: 25
                         width:25
                     }
                     Layout.fillWidth: true
                     implicitWidth: 300

                     implicitHeight: 50
                     onClicked: {
                         StartoverState.exit(0);
                     }
                 }
            }
        }
    }
}


