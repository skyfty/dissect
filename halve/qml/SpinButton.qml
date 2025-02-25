import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

ColumnLayout {
    id:root
    spacing: 1
    signal increase();
    signal decrease();

    Button {
        id:upButton

        focusPolicy:Qt.NoFocus
        padding: 0
        spacing: 0
        Layout.fillWidth: true
        Layout.preferredHeight: 8
        onClicked: {
            root.increase();
        }

        contentItem: IconLabel {
            spacing: 0
            icon {
                source: "qrc:/assets/images/up.png"
                height: 8
                width:20
            }
            color: "white"
        }
        background: Rectangle {
            radius: 0
            color: upButton.down ? "#46607c":upButton.hovered?"#464646":"#3c3c3c";
            visible: upButton.hovered
        }

        autoRepeat:true

    }
    Button {
        id:downButton

        focusPolicy:Qt.NoFocus
        Layout.fillWidth: true
        Layout.preferredHeight: 8
        padding: 0
        spacing: 0

        autoRepeat:true
        onClicked: {
            root.decrease();
        }

        contentItem: IconLabel {
            spacing: 0
            icon {
                source: "qrc:/assets/images/down.png"
                height: 8
                width:20
            }
            color: "white"
        }
        background: Rectangle {
            radius: 0
            color: downButton.down ? "#46607c":downButton.hovered?"#464646":"#3c3c3c";
            visible: downButton.hovered
        }
    }
}
