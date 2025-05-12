
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
    property bool isRegistrationRuning: false
    property bool isLoadingCtData: false
    FolderDialog {
        id: folderDialog
        onAccepted: {
            var fileUrl = new String(folderDialog.currentFolder);
            isLoadingCtData = true;
            registrationSourceStage.loadCtData(fileUrl.substring(8))
            isLoadingCtData = false;
        }
    }
    
    RegistrationProcessor {
        id:registrationProcessorInstance
    }

    ProgressReporter{
        id:progressReporterInstance
    }

    CtDataStorage {
        id:ctDataStorageInstance
        progressReporter:progressReporterInstance
    }

    function getDynamicSegmentNames() {
        return [
            "heart_atrium_left",          // 左心房
            "atrial_appendage_left",      // 左心耳
            "heart_ventricle_left",       // 左心室
            "pulmonary_vein",             // 肺静脉
            "heart_atrium_right",         // 右心房
            "heart_ventricle_right",      // 右心室
            "heart_myocardium",           //心肌
            "pulmonary_artery",           //肺动脉
            "aorta",                      // 主动脉
            "inferior_vena_cava",         // 下腔静脉
            "superior_vena_cava"          // 上腔静脉
        ]
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
                            source: "qrc:/assets/images/open_file.png"
                            height: 25
                            width:25
                        }
                        onClicked:  {
                            folderDialog.open();
                        }
                        enabled:!isLoadingCtData
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: isLoadingCtData ? qsTr("Loading CT data (Please wait...)"):qsTr("Open CT Folder")
                    }

                    ToolButton {
                        id: pointSelectBtn
                        checkable: true  
                        checked: registrationProcessorInstance.enablePick 
                        icon {
                            source: "qrc:/assets/images/select_points.png"
                            height: 25
                            width:25
                        }
                        onClicked:  {
                            registrationProcessorInstance.enablePick = !registrationProcessorInstance.enablePick;
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
                            registrationProcessorInstance.undo();
                            registrationSourceStage.updateOutlineStatus();
                            registrationTargetStage.updateOutlineStatus();
                        }
                        enabled:registrationProcessorInstance.enableUndo
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
                    registrationProcessor:registrationProcessorInstance
                    ctDataStorage:ctDataStorageInstance
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
                        property int expandedHeight: 275
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
                                text: segmentsPanel.isExpanded ? qsTr("Segments") + " ▼" : qsTr("Segments") + " ▶"
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
                                    interactive: false 
                                    model: ListModel {
                                        id: segmentModel
                                        Component.onCompleted: {
                                            let names = getDynamicSegmentNames()
                                            for (let i = 0; i < names.length; i++) {
                                                append({
                                                    "isChecked": registrationSourceStage.visibleStates[i],
                                                    "segmentName": names[i] 
                                                })
                                            }
                                        }
                                    }
                                    delegate: CheckBox {
                                        width: ListView.view.width
                                        text: qsTr(model.segmentName)
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
                        registrationProcessorInstance.undoType = 1; 
                    }
                }
                RegistrationTargetStage {
                    id: registrationTargetStage
                    profile:root.profile
                    registrationProcessor:registrationProcessorInstance
                    SplitView.fillWidth: true 
                    SplitView.minimumWidth: 100
                    onPickFinished : { 
                        registrationSourceStage.updateOutlineStatus();
                        registrationTargetStage.updateOutlineStatus();
                        registrationProcessorInstance.undoType = -1;
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
                        value: progressReporterInstance.progressValue
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
                            Text {
                                anchors.centerIn: parent
                                text: qsTr(progressReporterInstance.progressText)
                                color: "#333333" 
                                font.pixelSize: 12
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }
                    Button {
                        text: qsTr("Start Registration")
                        flat: false
                        enabled:registrationProcessorInstance.enableRegistration&&(!isRegistrationRuning)
                        onClicked: { 
                            isRegistrationRuning = true;
                            Qt.callLater(function() { //refresh UI
                                registrationProcessorInstance.startRegistration(profile,obscurity,registrationSourceStage.visibleStates);
                                isRegistrationRuning=false;
                            });
                        }
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
