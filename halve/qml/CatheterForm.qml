
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels 1.0
import QtQuick.Window
import "models"

Item {
    id:root
    required property var catheter
    CatheterShow {
        id:stage
        anchors.fill: parent
        catheter:root.catheter
        onInitRender:stage.resetRender()
    }

    ElectrodeModel{
        id:electrodeModel
    }
    Item {
        height: gridLayout.implicitHeight + 8
        width: gridLayout.implicitWidth + 8
        x:4
        y:4
        Rectangle {
            anchors.fill: parent
            color:"#333333"
            opacity: 0.6
            radius:3
            border.color: "gray"
            border.width: 1
        }
        Item {
            anchors.fill: parent
            anchors.margins: 4
            GridLayout {
                id:gridLayout
                columns: 4

                Label {
                    text:qsTr("Magnetic")
                    Layout.alignment: Qt.AlignRight

                }
                Label {
                    anchors.margins: 1
                    Layout.alignment: Qt.AlignLeft
                    text: qsTr(catheter.magnetism?"Yes":"No")
                    font.bold: true
                    verticalAlignment: TextInput.AlignVCenter
                }
                Label {
                    text:qsTr("Amount")
                    Layout.alignment: Qt.AlignRight

                }
                Label {
                    anchors.margins: 1
                    Layout.alignment: Qt.AlignLeft
                    text: catheter.amount
                    font.bold: true
                    verticalAlignment: TextInput.AlignVCenter
                }

                Label {
                    text:qsTr("Manufacturer")
                    Layout.alignment: Qt.AlignRight
                }
                Label {
                    anchors.margins: 1
                    Layout.alignment: Qt.AlignLeft
                    text: catheter.manufacturer
                    font.bold: true
                    verticalAlignment: TextInput.AlignVCenter

                }

                Label {
                    text:qsTr("Brand")
                    Layout.alignment: Qt.AlignRight
                }
                Label {
                    anchors.margins: 1
                    Layout.alignment: Qt.AlignLeft
                    text: catheter.name
                    font.bold: true
                    verticalAlignment: TextInput.AlignVCenter
                }


                Label {
                    text:qsTr("Model")
                    Layout.alignment: Qt.AlignRight
                }
                Label {
                    anchors.margins: 1
                    text: catheter.version
                    Layout.alignment: Qt.AlignLeft
                    font.bold: true
                    verticalAlignment: TextInput.AlignVCenter
                }


                Label {
                    text:qsTr("Fr")
                    Layout.alignment: Qt.AlignRight
                }
                Label {
                    anchors.margins: 1
                    Layout.alignment: Qt.AlignLeft
                    text: catheter.fr
                    font.bold: true
                    verticalAlignment: TextInput.AlignVCenter
                }

                Label {
                    text:qsTr("Nodes")
                    Layout.alignment: Qt.AlignRight
                }
                Label {
                    anchors.margins: 1
                    Layout.alignment: Qt.AlignLeft
                    text: catheter.nodes
                    font.bold: true
                    verticalAlignment: TextInput.AlignVCenter
                }
                Label {
                    text:qsTr("diameter (mm)")
                    Layout.alignment: Qt.AlignRight
                }
                Label {
                    anchors.margins: 1
                    text: catheter.diameter
                    Layout.alignment: Qt.AlignLeft
                    font.bold: true
                    verticalAlignment: TextInput.AlignVCenter
                }

                Label {
                    text:qsTr("Distal length")
                    Layout.alignment: Qt.AlignRight
                }
                Label {
                    anchors.margins: 1
                    Layout.alignment: Qt.AlignLeft
                    text: catheter.far
                    font.bold: true
                    verticalAlignment: TextInput.AlignVCenter
                }

                Label {
                    text:qsTr("Electrode length")
                    Layout.alignment: Qt.AlignRight
                }
                Label {
                    anchors.margins: 1
                    Layout.alignment: Qt.AlignLeft
                    text: catheter.electrodeLength
                    font.bold: true
                    verticalAlignment: TextInput.AlignVCenter
                }
                
                Label {
                    text:qsTr("Status")
                    Layout.alignment: Qt.AlignRight
                }
                Label {
                    anchors.margins: 1
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                    text: (catheter.valid ? qsTr("Valid") :qsTr("Invalid"))
                    font.bold: true
                    color: (catheter.valid ? "white" : "red")
                    verticalAlignment: TextInput.AlignVCenter
                }
                
                Label {
                    text:qsTr("Electrode params")
                    Layout.alignment: Qt.AlignRight
                }
                Label {
                    anchors.margins: 1
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                    text: (catheter.magnetism && catheter.amount > 0 ? catheter.magnetismAmbient.description :"")
                    font.bold: true
                    verticalAlignment: TextInput.AlignVCenter
                    Layout.columnSpan: 3
                }
            }

        }

    }



}
