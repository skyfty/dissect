import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtCharts
import Qt.labs.platform  as Platform

Page {
    id: root
    property alias model:userListModel.sourceModel

    UserListSortFilterProxyModel {
        id:userListModel
    }

    function startStudy() {
        if (ProfileManager.open(userListModel.getUser(userTable.currentRow).id)) {
            ProfileManager.startStudy()
        }
    }

    header:ToolBar {
        RowLayout {
            anchors.fill: parent
            ToolButton {
                id: cancelBtn
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
                icon {
                    source: "qrc:/assets/images/user_add.png"
                    height: 25
                    width:25
                }
                onClicked: {
                    root.StackView.view.push("UserCreatePage.qml",{model:userListModel});
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Create new users")
            }
            ToolButton {
                Layout.margins: 3
                icon {
                    source: "qrc:/assets/images/user_edit.png"
                    height: 25
                    width:25
                }
                onClicked:  {
                    root.StackView.view.push("UserEditPage.qml",{
                         model:userListModel,
                         userData:userListModel.getUser(userTable.currentRow)
                     });
                }

                enabled: userTable.currentRow !== -1
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Edit user")
            }
            ToolButton {
                Layout.margins: 3
                icon {
                    source: "qrc:/assets/images/user_delete.png"
                    height: 25
                    width:25
                }
                onClicked:  {
                    deleteConfirmDialog.open();
                }
                enabled: userTable.currentRow !== -1


                ConfirmDialog {
                    id: deleteConfirmDialog
                    text: qsTr("Are you sure to delete this?")
                    onAccepted: {
                        userTable.model.removeRows(userTable.currentRow, 1)
                    }
                }

                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Delete")
            }


            ToolButton {
                Layout.margins: 3
                icon {
                    source: "qrc:/assets/images/shenhe.png"
                    height: 25
                    width:25
                }
                onClicked: startStudy()
                enabled: userTable.currentRow !== -1
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Entry device setting")
            }
            ToolSeparator {}
            ToolButton {
                Layout.margins: 3
                icon {
                    source: "qrc:/assets/images/settings.png"
                    height: 25
                    width:25
                }
                onClicked:  {
                    root.StackView.view.push("SettingsPage.qml");

                }
            }

            ToolButton {
                Layout.margins: 3
                icon {
                    source: "qrc:/assets/images/export.png"
                    height: 25
                    width:25
                }
                Platform.FolderDialog {
                    id: exportDialog;
                    property var exportGuids: []
                    title: qsTr("Please choose a file");
                    onAccepted: {
                        var fileUrl = new String(exportDialog.folder);
                        var userList = [userListModel.getUser(userTable.currentRow).id];
                        userExporter.exportUsersToPath(userList, fileUrl.substring(8));
                    }
                }

                UserExporter {
                    id: userExporter
                    onFinished: {
                        Utils.showMessage(mainWindow, qsTr("Case data export was completed"));
                    }
                }
                enabled: userTable.currentRow !== -1 &&  !userExporter.isRunning
                onClicked:  {
                    exportDialog.open()
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: "导出"
            }
            Item {
                Layout.fillWidth: true
            }
            SearchInput {
                id:searchInput
                Layout.fillHeight: true
                Layout.minimumWidth:250
                Layout.margins: 5
                model: userTable.model
            }
            ToolButton {
                icon {
                    source: "qrc:/assets/images/quit.png"
                    height: 25
                    width:25
                }
                onClicked: {
                    Qt.quit()
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Quit")
            }
        }
    }

    UserTable {
        id:userTable
        anchors.fill:parent
        model:userListModel
        onDoubleClick: startStudy()
    }
}
