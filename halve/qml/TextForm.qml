import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Halve

ColumnLayout {
    id:root
    property alias text: name.text
    property alias placeholderText: name.placeholderText
    signal accepted();
    Layout.fillWidth: true
    Layout.fillHeight: true
    RowLayout {
        TextField {
            id: name
            Layout.fillHeight: true
            focus: true
            placeholderText: qsTr("Input name")
            font.bold: true
            placeholderTextColor:"#9f9f9f"
        }

        Button {
            text:qsTr("Save")
            font.bold: true
            Layout.fillHeight: true
             onClicked:  {
                 accepted();
             }
        }
    }
}
