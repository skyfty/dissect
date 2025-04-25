
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels 1.0
import QtQuick.Window

Item {
    id:root
    required property var profile
    required property var obscurity
    required property var combined
    required property var channel
    required property var reseauListModel
    required property var options

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
                            height: 30
                            width:30
                        }
                        onClicked:  {
                            //catheterRepertoryTableModel.insertRows(0, 1);
                        }
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Open File")
                    }

                    ToolButton {
                        icon {
                            source: "qrc:/assets/images/select_points.png"
                            height: 30
                            width:30
                        }
                        onClicked:  {
                            //catheterRepertoryTableModel.insertRows(0, 1);
                        }
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Select Points")
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
                    SplitView.preferredWidth: parent.width*0.5
                    Layout.minimumWidth: 100
                }
                RegistrationTargetStage {
                    id: registrationTargetStage
                    SplitView.fillWidth: true 
                    Layout.minimumWidth: 100
                }
            }

            footer: ToolBar {
                height: 80
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
                        onClicked: { /* Registration logic */ }
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
