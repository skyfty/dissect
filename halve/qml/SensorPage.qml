import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve 1.0
import com.kdab.dockwidgets 1.0 as KDDW
import Qt.labs.platform

Page {
    id: root
    ColumnLayout {
        anchors.fill: parent
        ToolBar {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter|Qt.AlignTop

            RowLayout {
                spacing: 1
                anchors.fill: parent
                PageTitle {
                    title:"传感器设置"
                    image:"qrc:/assets/images/sensor.png"
                }
                ToolButton {
                    icon {
                        source: "qrc:/assets/images/back.png"
                        height: 25
                        width:25
                    }
                    onClicked:  {
                        root.StackView.view.pop();
                    }
                }

                Item {
                    Layout.fillWidth: true
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
                    ToolTip.text: qsTr("退出")
                }
            }
        }
        Item {
            Layout.alignment: Qt.AlignCenter|Qt.AlignTop
            Layout.fillWidth: true
            Layout.fillHeight: true

            Pane {
                background: Rectangle {
                    color:"#545454"
                    radius:4
                }

                anchors.centerIn: parent
                ColumnLayout {
                    anchors.centerIn: parent


                }
            }
        }
    }
}
