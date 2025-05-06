
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels 1.0
import QtQuick.Window
import QtQuick.Dialogs

Item {
    id:root
    required property var profile
    required property var obscurity

    FolderDialog {
        id: folderDialog
        onAccepted: {
            var fileUrl = new String(folderDialog.currentFolder);
            registrationSourceStage.loadCtData(fileUrl.substring(8))
        }
    }
    
    RegistrationProcessor {
        id:regProc
    }

    readonly property var segmentNames: [
        qsTr("heart_atrium_left"),          // 左心房
        qsTr("atrial_appendage_left"),      // 左心耳
        qsTr("heart_ventricle_left"),       // 左心室
        qsTr("pulmonary_vein"),             // 肺静脉
        qsTr("heart_atrium_right"),         // 右心房
        qsTr("heart_ventricle_right"),      // 右心室
        qsTr("heart_myocardium"),
        qsTr("pulmonary_artery"),          // 肺动脉
        qsTr("aorta"),                      // 主动脉
        qsTr("inferior_vena_cava"),         // 下腔静脉
        qsTr("superior_vena_cava")          // 上腔静脉
    ]
    
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
                            source: "qrc:/assets/images/open_file.png"
                            height: 25
                            width:25
                        }
                        onClicked:  {
                            folderDialog.open();
                        }
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Open CT Folder")
                    }

                    ToolButton {
                        id: pointSelectBtn
                        checkable: true  
                        checked: registrationSourceStage.enablePick 
                        icon {
                            source: "qrc:/assets/images/select_points.png"
                            height: 25
                            width:25
                        }
                        onClicked:  {
                            registrationSourceStage.enablePick = !registrationSourceStage.enablePick;
                            registrationTargetStage.enablePick = !registrationTargetStage.enablePick;
                        }
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Select Points")

                    }

                    ToolButton {
                        id: pointClearBtn
                        icon {
                            source: "qrc:/assets/images/clear_select_points.png"
                            height: 25
                            width:25
                        }
                        onClicked:  {
                            registrationSourceStage.clearSelectedPoints();
                            registrationTargetStage.clearSelectedPoints();
                        }
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Clear Selected Points")
                    }

                    ToolButton {
                        id: undoBtn
                        icon {
                            source: "qrc:/assets/images/undo.svg"
                            height: 25
                            width:25
                        }
                        onClicked:  {
                            regProc.undo();
                            registrationSourceStage.updateOutlineStatus();
                            registrationTargetStage.updateOutlineStatus();
                        }
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Undo")
                    }
                    
                    Item {
                        Layout.fillWidth: true
                    }
                }
            }

            SplitView {
                anchors.fill: parent
                orientation: Qt.Horizontal
                
                RegistrationSourceStage {
                    id: registrationSourceStage
                    profile:root.profile
                    registrationProcessor:regProc
                    SplitView.preferredWidth: parent.width*0.5
                    SplitView.minimumWidth: 100
                    Rectangle {
                        id: segmentsPanel
                        width: 220
                        anchors { left: parent.left; top: parent.top; margins: 10 }
                        color: "#404040"
                        radius: 8
                        opacity: 0.6
                        layer.enabled: true

                        // 展开状态控制
                        property bool isExpanded: true
                        property int collapsedHeight: 35
                        property int expandedHeight: 265
                        height: isExpanded ? expandedHeight : collapsedHeight

                        Behavior on height {
                            NumberAnimation { duration: 200; easing.type: Easing.OutQuad }
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            spacing: 3

                            Label {
                                Layout.fillWidth: true
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: segmentsPanel.isExpanded = !segmentsPanel.isExpanded
                                }
                                text: segmentsPanel.isExpanded ? "Segments ▼" : "Segments ▶"
                                font.bold: true
                                font.pixelSize: 18
                                horizontalAlignment: Text.AlignHCenter
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                implicitHeight: segmentsPanel.isExpanded ? parent.height - 50 : 0
                                clip: true

                                ListView {
                                    anchors.fill: parent
                                    model: ListModel {
                                        id: segmentModel
                                        Component.onCompleted: {
                                            for (let i = 0; i < root.segmentNames.length; i++) {
                                                append({
                                                    "isChecked": registrationSourceStage.visibleStates[i],
                                                    "segmentName": root.segmentNames[i]
                                                })
                                            }
                                        }
                                    }
                                    delegate: CheckBox {
                                        width: ListView.view.width
                                        text: model.segmentName
                                        font { pixelSize: 17; bold: true }
                                        checked: model.isChecked
                                        onCheckedChanged: registrationSourceStage.visibleStates[index] = checked
                                    }
                                }

                                Behavior on implicitHeight {
                                    NumberAnimation { duration: 200; easing.type: Easing.OutQuad }
                                }
                            }
                        }
                    }
                    onPickFinished : {
                        registrationSourceStage.updateOutlineStatus();
                        registrationTargetStage.updateOutlineStatus();
                        regProc.undoType = 1; 
                    }
                }
                RegistrationTargetStage {
                    id: registrationTargetStage
                    profile:root.profile
                    registrationProcessor:regProc
                    SplitView.fillWidth: true 
                    SplitView.minimumWidth: 100
                    onPickFinished : { 
                        registrationSourceStage.updateOutlineStatus();
                        registrationTargetStage.updateOutlineStatus();
                        regProc.undoType = -1; 
                    }
                }
            }

            footer: ToolBar {
                height: 70
                ColumnLayout  {
                    anchors.fill: parent
                    spacing: 10
                    ProgressBar {
                        id: registrationProgress
                        Layout.fillWidth: true
                        Layout.preferredHeight: 20
                        value: 0.5
                        background: Rectangle {
                            implicitHeight: 8
                            radius: 4
                            color: "#e0e0e0"
                        }
                        contentItem: Item {
                            Rectangle {
                                width: registrationProgress.visualPosition * parent.width
                                height: parent.height
                                radius: 4
                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: "#4CAF50" }
                                    GradientStop { position: 1.0; color: "#81C784" }
                                }
                            }
                        }
                    }
                    Button {
                        text: qsTr("Start Registration")
                        flat: false
                        onClicked: { regProc.startRegistration(profile,obscurity,registrationSourceStage.visibleStates); }
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Start Registration")
                        font {
                            pixelSize: 20
                            bold: true
                        }
                        Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
                    }
                }
            }
        }
    }

}
