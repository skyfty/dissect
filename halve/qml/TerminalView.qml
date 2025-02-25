import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtCharts 2.5
import QtQuick.Dialogs

Item {
    id:root

    Alleyway {
        id:alleyway
        fpgaAddress:AppSettings.fpgaAddress
        fpgaPort:AppSettings.fpgaPort
        port:AppSettings.controlPort
        onPortChanged: {
            alleyway.reconnect();
        }
        onError:(message)=> {
            logArea.append("<span style='color: red;'>" + message + "</span>");
        }
        onDataReady: (message)=> {
            logArea.append("<span style='color: white;'>" + message + "</span>");
         }
        Component.onCompleted:{
            alleyway.connect()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            id:toolbar
            Layout.fillWidth: true
            Layout.maximumHeight:  34
            Layout.rightMargin: 3
            Layout.leftMargin: 3
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/clean.png"
                    height: 19
                    width:19
                }
                action: Action {
                    onTriggered: logArea.clear()
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Consult Settings")
            }

            ToolSeparator {}

            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/turnon.svg"
                    height: 19
                    width:19
                }
                action: Action {
                    onTriggered: {
                        alleyway.turnon();
                        log("trunon");
                    }
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Trun on")
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/turnoff.svg"
                    height: 19
                    width:19
                }
                action: Action {
                    onTriggered:{
                        alleyway.turnoff();
                        log("turnoff");
                    }
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Trun off")
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/save.png"
                    height: 19
                    width:19
                }
                action: Action {
                    onTriggered: {
                        alleyway.save();
                        log("save");
                    }
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Trun off")
            }
        }
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            spacing: 4

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                TextArea {
                    id: logArea
                    textFormat: Text.RichText
                    focus: true
                    readOnly: true
                }
             }
            RowLayout {
                Layout.fillWidth: true
                spacing: 3
                Layout.preferredHeight: 60
                Layout.minimumHeight: 60
                Layout.maximumHeight: 60
                Layout.margins: 2
                 TextArea {
                     id: inputField

                     Layout.fillWidth: true
                     Layout.fillHeight: true

                     color: "black"
                     // 使用 Rectangle 作为背景来实现边框效果
                     background: Rectangle {
                         color: "white" // 背景颜色
                         border.color: "black" // 边框颜色
                         border.width: 2 // 边框宽度
                         radius: 0 // 边角圆滑度
                     }
                 }

                Button {
                    text: "Send"
                    Layout.fillHeight: true
                    onClicked: {
                        processCommand(inputField.text);
                        inputField.text = ""; // 清空输入框
                    }
                }
            }
        }
    }
    function log(message) {
          logArea.append(message + "\n");
      }
    function processCommand(command) {
        // 这里处理输入的命令
        // 为了演示，我们将命令直接输出到控制台
        alleyway.send(command);
        log("> " + command);
        // 你可以在这里添加更多的命令处理逻辑
    }
}
