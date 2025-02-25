import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve

Menu {
    id: root
    property alias text:nameTextInput.text
    property alias maximumLength:nameTextInput.maximumLength
    property alias placeholderText:nameTextInput.placeholderText

    signal accepted();

    contentItem:Pane {
        focusPolicy: Qt.ClickFocus
        clip: true
        RowLayout {
            anchors.fill: parent
            TextField {
                id:nameTextInput
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: "white"
                focus: true
                placeholderTextColor:"white"
                onAccepted: {
                    root.close();
                }
            }
        }
    }
    onAboutToHide: {
        accepted();
        root.close()
    }

    onAboutToShow: {
        nameTextInput.forceActiveFocus();
    }
}
