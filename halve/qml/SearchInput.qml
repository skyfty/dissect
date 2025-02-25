import QtQuick 2.0
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id:root
    required property var model
    property alias filterText: searchTextField.text
    function search() {
        model.setFilterFixedString(searchTextField.text.trim());
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        radius:2

        RowLayout {
            anchors.fill: parent
            spacing: 0
            TextField {
                id:searchTextField
                Layout.leftMargin: 5
                background: Rectangle {
                    border.width: 0
                    color: "black"
                }

                placeholderText:qsTr("Search keyword")
                placeholderTextColor:"white"
                Layout.fillWidth: true
                onTextEdited: {
                    model.setFilterFixedString(this.text.trim());
                }

            }

            ToolButton {

                icon {
                    source: "qrc:/assets/images/clean.png"
                    height: 14
                    width:14
                }
                onClicked: {
                    searchTextField.clear();
                    model.setFilterFixedString(this.text.trim())
                }
            }

        }
    }

}
