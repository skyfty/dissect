import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Dialogs
import "colorpicker"

Rectangle {
    signal accepted(var color)
    signal changed(var color)
    property bool enabled:true
    id:colorPane
    radius: 3
    border {
        width:1
        color:"#696969"
    }

    MouseArea {
        anchors.fill: parent
        enabled: colorPane.enabled
        onClicked: {
            colorPickerPopupComponent.createObject(parent).open()

        }
    }

    Component {
        id:colorPickerPopupComponent
        Menu {
            id: colorPickerPopup
            y: parent.height
            contentItem: Pane {
                focusPolicy: Qt.ClickFocus
                padding: 0
                ColumnLayout {
                    anchors.fill: parent
                    ColorPicker {
                        id: colorPicker
                        property bool init: false
                        width: 280
                        height: 200
                        enableDetails:false
                        enableAlphaChannel:false
                        onColorChanged: (changedColor) => {
                            if (colorPicker.init) {
                                colorPane.color = changedColor;
                                changed(changedColor)
                            }
                        }

                        Component.onCompleted: {
                            colorPicker.setColor(colorPane.color)
                            colorPicker.init = true;
                        }
                    }
                }
            }
            onAboutToHide: {
                colorPickerPopup.destroy();
                colorPane.accepted(colorPane.color);
            }
        }
    }
}

