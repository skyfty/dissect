
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels 1.0
import QtQuick.Window

Item {
    id:root
    required property var model
    required property var row

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing:5


        ColumnLayout {
            Layout.fillWidth:true
            Label {
                text:qsTr("Description")
            }
            TextField  {
                id:descriptionTextArea
                Layout.fillWidth:true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignLeft
                verticalAlignment:Text.AlignTop
                placeholderText: qsTr("Enter description")
                text: model.data(model.index(row, 0), DissolveListModel.DESCRIPTION)
                font.pixelSize: 15
                onEditingFinished:{
                }
                onTextEdited: {
                    model.setData(model.index(row, 0), text, DissolveListModel.DESCRIPTION)

                }

                wrapMode: Text.Wrap
                focus: true
                background:Rectangle {
                    radius: 5
                    color:"#181818"
                    border.color: descriptionTextArea.activeFocus ? "#0078d4" : "#5f5f5f"
                }

                Timer {
                     interval: 100; running: true; repeat: false
                     onTriggered: descriptionTextArea.forceActiveFocus()
                 }
            }
            Layout.fillHeight: true
        }


    }

}
