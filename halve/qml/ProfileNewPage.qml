import QtCore
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtQuick
import QtQuick.Controls
import QtQuick.Controls 2.15
import "models"

Item {
    id:root
    Image {
        source: "qrc:/assets/images/microscope.png"
        width:700
        height: 700
        opacity:0.02
        anchors.bottom: parent.bottom
        anchors.left: parent.left

    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 12


        ColumnLayout {
            spacing: 1
            Label {
                font.pixelSize: 20
                font.bold: true
                text:qsTr("Mode")
            }

            ComboBox {
                id:channelMode
                implicitHeight: 40
                implicitWidth:  300
                Layout.alignment: Qt.AlignLeft
                textRole:"text"
                valueRole:"value"
                model: ChannelModeModel{}
            }
        }

        ColumnLayout {
            spacing: 1
            Label {
                font.pixelSize: 20
                font.bold: true
                text:qsTr("The surgeon")


            }
            TextField {
                id:doctor
                implicitWidth:  300
                implicitHeight: 40
                font.pixelSize: 17
                font.bold: true
                text:ProfileManager.currentUser.doctor
            }
        }


        ColumnLayout {
            spacing: 1
            Label {
                font.pixelSize: 20
                font.bold: true
                text:qsTr("Date")
            }
            TextField {
                property var currentDate: new Date()

                id:surgicalDate
                implicitWidth:  300
                implicitHeight: 40
                font.pixelSize: 17
                font.bold: true
                readOnly: true
                text: currentDate.toLocaleDateString(Qt.locale(InterfaceSettings.language))

                MouseArea {
                    anchors.fill: parent;
                    onClicked: {
                       datePicker.open()
                    }
                }
            }

            Popup {
                id:datePicker
                padding: 10
                contentItem: DatePicker{
                    selectedDate:surgicalDate.currentDate
                    minDate:new Date(new Date().getFullYear() - 5, 1, 1)
                    maxDate:new Date(new Date().getFullYear() + 5, 1, 1)
                    onFinished:(selectedDate)=> {
                        surgicalDate.currentDate = selectedDate;
                        datePicker.close()
                    }
                }
            }

        }
        // ColumnLayout {
        //     spacing: 1
        //     Label {
        //         font.pixelSize: 20
        //         font.bold: true
        //         text:qsTr("PODx")
        //     }
        //     TextField {
        //         implicitWidth:  300
        //         implicitHeight: 40
        //         font.pixelSize: 17
        //         font.bold: true
        //     }
        // }
        Button {
            text:qsTr("New surgery")
            Layout.fillWidth: true
            implicitHeight: 50
            font.pixelSize: 17
            font.bold: true
            enabled: doctor.text !== ""
            onClicked: {
                root.StackView.view.pop();
                ProfileManager.addProfile(doctor.text, surgicalDate.currentDate, channelMode.currentValue)
            }
        }
        Button {
            text:qsTr("Back")
            Layout.fillWidth: true
            implicitHeight: 50
            font.pixelSize: 17
            font.bold: true
            onClicked: {
                root.StackView.view.pop();
            }
        }
    }
}
