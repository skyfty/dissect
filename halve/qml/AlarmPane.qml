import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve

Pane {
    id:root
    property alias text:label.text
    property alias backgroundOpacity:backgroundRectangle.opacity
    property alias backgroundColor:backgroundRectangle.color

    signal close()

    contentWidth: label.implicitWidth
    contentHeight: label.implicitHeight
    background: Rectangle {
        id:backgroundRectangle
        radius:3
        border.color: "gray"
        border.width: 2
    }

    states: [
        State {
            name: Halve.AT_INFO
            PropertyChanges {
                label {
                    color: "White"
                }
                backgroundRectangle {
                   color: "#333333"
                }
                closeButton {
                    visible: true
                }

            }
        },
        State {
            name: Halve.AT_WARNING
            PropertyChanges {
                label {
                    color: "White"
                }

                closeButton {
                    visible: true
                }
                backgroundRectangle {
                   color: "#F2C883"
                }
            }
        },
        State {
            name: Halve.AT_ERROR
            PropertyChanges {
                label {
                    color: "White"
                }

                closeButton {
                    visible: false
                }
                backgroundRectangle {
                   color: "#FA7B6A"
                }
            }
        }
    ]
    RowLayout {
        width: parent.width
        Label {
            id:label
            font.family: "Helvetica"
            font.pointSize: 10
            font.bold: true
            font.weight: 600
            wrapMode:Text.Wrap
            Layout.fillHeight: true
            Layout.fillWidth: true

        }

        ToolButton {
            id:closeButton
            Layout.alignment: Qt.AlignRight|Qt.AlignTop
            icon {
                source: "qrc:/assets/images/back.png"
                height: 15
                width:15
            }

            background: Rectangle {
                id: panel
                implicitWidth: 15
                implicitHeight: 15
                color:  closeButton.hovered?"#464646":"transparent"
                radius: 2
            }
            onClicked: root.close()
        }
    }

}
