import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import QtQml

Rectangle {
    id:root
    property int from: 0
    property real stepSize:1
    property int to: 100
    property int value: 0
    property alias live: slider.live
    property bool editable: true
    color:"#282828"
    border.width: root.visualFocus ? 2 : 1
    radius: 1
    height: 30
    border.color: comboBox.activeFocus ? "#0078d4" : "#212133"

    signal valueModified();


    onValueChanged: {
        spinBox.value = root.value;
        spinBox.valueModified();
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 1
        spacing: 0

        ComboBox {
            id:comboBox
            Layout.fillHeight: true
            Layout.fillWidth: true

            contentItem: SpinBox {
                id:spinBox
                editable:false
                clip: true
                value: root.value
                to: root.to
                stepSize: root.stepSize
                from: root.from
                topPadding: 4
                leftPadding: 4 - comboBox.padding
                rightPadding: 30
                bottomPadding: 4
                font.pixelSize: 13
                validator: IntValidator {
                    bottom: Math.min(spinBox.from, spinBox.to)
                    top:  Math.max(spinBox.from, spinBox.to)
                }
                onValueModified: {
                    root.value = value;
                    slider.value = root.value
                    root.valueModified();
                }

                contentItem:  Control {
                    z: 2
                    clip: width < implicitWidth
                    Label {
                        anchors.fill: parent
                        text: spinBox.displayText
                        font: spinBox.font
                        color: spinBox.palette.text
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        clip: width < implicitWidth


                        TapHandler {
                            enabled: root.editable
                            onTapped: {
                                textInputPopup.open();
                            }
                        }

                        Menu {
                            id: textInputPopup
                            x:0
                            y:0
                            height: parent.height
                            width: parent.width

                            background:  Rectangle {
                                color: "#282828"
                            }
                            contentItem: TextInput {
                                id:textInput
                                font: spinBox.font
                                color: spinBox.palette.text
                                selectionColor: spinBox.palette.highlight
                                selectedTextColor: spinBox.palette.highlightedText
                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                                clip: width < implicitWidth
                                onTextEdited: {
                                    slider.value = Number(text);
                                }
                                validator: RegularExpressionValidator { regularExpression: /^[+-]?(0|[1-9]\d*)$/   }
                            }
                            onAboutToShow: {
                                textInput.text = spinBox.displayText
                                textInput.forceActiveFocus();
                            }

                            onAboutToHide: {

                                spinBox.value = slider.value;
                                spinBox.valueModified();
                            }
                        }
                    }



                }

                background: Rectangle {
                    clip: true
                    radius: 3
                    color:"#282828"
                }
                textFromValue: function(value, locale) {
                    return Number(value).toString();
                 }

                 valueFromText: function(text, locale) {
                     return Number.fromLocaleString(locale, text)
                 }
                up.indicator: PaddedRectangle {
                    x: spinBox.width - width - 1
                    y: 1
                    height: spinBox.height / 2 - 1
                    implicitWidth: 16
                    implicitHeight: 10



                    radius: 1.7
                    clip: true
                    topPadding: -2
                    leftPadding: -2
                    color: "transparent"

                    ColorImage {
                        scale: -1
                        width: parent.width
                        height: parent.height
                        opacity: enabled ? 1.0 : 0.5
                        color: "white"
                        source: "qrc:/qt-project.org/imports/QtQuick/Controls/Fusion/images/arrow.png"
                        fillMode: Image.Pad
                    }
                }

                down.indicator: PaddedRectangle {
                    x: spinBox.width - width - 1
                    y: spinBox.height - height - 1
                    height: spinBox.height / 2 - 1
                    implicitWidth: 16
                    implicitHeight: 10

                    radius: 1.7
                    clip: true
                    topPadding: -2
                    leftPadding: -2
                    color: "transparent"

                    ColorImage {
                        width: parent.width
                        height: parent.height
                        opacity: enabled ? 1.0 : 0.5
                        color: "white"
                        source: "qrc:/qt-project.org/imports/QtQuick/Controls/Fusion/images/arrow.png"
                        fillMode: Image.Pad
                    }
                }
            }

            background: Item {
                implicitWidth: 120
                implicitHeight: 24
            }
            popup: Menu {
                width: comboBox.width
                y: parent.height
                height: 30

                background:  Rectangle {
                    color: "#333333"
                    border.color: "#535353"
                    radius: 3
                }
                contentItem:Pane {
                    Slider {
                        id:slider
                        value: root.value
                        from: root.from
                        to: root.to
                        stepSize: root.stepSize
                        anchors.fill: parent
                        onMoved: {
                            spinBox.value = value;
                            spinBox.valueModified();
                        }

                    }
                }
            }
        }

    }

}
