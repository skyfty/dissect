import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Layouts
import QtQml

Rectangle {
    id:root
    property real from: 0.00
    property real stepSize:0.01
    property real to: 1.00
    property real value: 0.00
    property alias live: slider.live
    property int decimals: 2
    property bool editable: true
    readonly property int decimalFactor: Math.pow(10, decimals)
    color:"#282828"
    border.width: root.visualFocus ? 2 : 1
    radius: 1
    height: 30
    border.color: comboBox.activeFocus ? "#0078d4" : "#212133"

    signal valueModified();

    onValueChanged: {
        spinBox.value = root.decimalToInt(root.value);
        spinBox.valueModified();
    }

    function decimalToInt(decimal) {
        return Math.round((decimal + Number.EPSILON) * root.decimalFactor);
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
                value: root.decimalToInt(root.value)
                to: root.decimalToInt(root.to)
                stepSize: root.decimalToInt(root.stepSize)
                from: root.decimalToInt(root.from)
                topPadding: 4
                leftPadding: 4 - comboBox.padding
                rightPadding: 30
                bottomPadding: 4
                font.pixelSize: 13
                validator: DoubleValidator {
                    bottom: Math.min(spinBox.from, spinBox.to)
                    top:  Math.max(spinBox.from, spinBox.to)
                    decimals: root.decimals
                    notation: DoubleValidator.StandardNotation
                }
                onValueModified: {
                    root.value = value / root.decimalFactor;
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
                                validator: RegularExpressionValidator { regularExpression: /^[+-]?(\d+(\.\d*)?|\.\d+)$/ }
                            }
                            onAboutToShow: {
                                textInput.text = spinBox.displayText
                            }

                            onAboutToHide: {

                                spinBox.value = Math.round((slider.value + Number.EPSILON) * root.decimalFactor);
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
                    return Number(value / root.decimalFactor).toLocaleString(locale, 'f', root.decimals);
                 }

                 valueFromText: function(text, locale) {
                     return Math.round(Number.fromLocaleString(locale, text) * root.decimalFactor)
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
                            spinBox.value = root.decimalToInt(value);
                            spinBox.valueModified();
                        }

                    }
                }
            }
        }

    }

}
