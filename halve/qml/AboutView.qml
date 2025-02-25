
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import QtQuick.Window

Menu {
    id: root
    background: Rectangle {
        color: "#191919"
        border.color: "#282828"
        border.width: 1
    }
    contentItem:Pane {
        focusPolicy: Qt.ClickFocus

        RowLayout {
            anchors.fill: parent
            Image {
                 source: "qrc:/assets/images/centercode.svg"

                 Layout.preferredHeight: 100
                 Layout.preferredWidth: 100
            }
            ColumnLayout{
                Label{
                    font.pixelSize: 15
                    font.bold: true
                    text:"Medical Treatment " + Qt.application.version
                }
                Label{
                    text:"Based on QT Qt 6.6.2 (MSVC 2019, x86_64)"
                }
                Label{
                    text:"Copyright 2022-2024 The xxxx Company Ltd. All rights revered!"
                }
                Item {
                    Layout.fillHeight: true
                }
            }



        }
    }
    onAboutToHide: {
        root.close()
    }
}
