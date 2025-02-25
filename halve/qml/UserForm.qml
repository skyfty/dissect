import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Halve

Item {
    id:root
    property User initUserData
    signal finished();
    signal cancel();
    function showIdCardError() {
        idcardToolTip.visible = true;
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 12

        RowLayout {
            spacing: 12

            ColumnLayout {
                spacing: 12

                ColumnLayout {
                    spacing: 1
                    Label {
                        font.pixelSize: 20
                        font.bold: true
                        text:qsTr("Name(*)")
                    }

                    TextField {
                        id: name
                        focus: true
                        implicitWidth:  400
                        implicitHeight: 40
                        font.pixelSize: 17
                        font.bold: true
                        text: initUserData.name
                        Layout.alignment: Qt.AlignLeft
                        Binding {
                            target: initUserData;
                            property: "name";
                            value: name.text
                        }
                    }

                }
                ColumnLayout {
                    spacing: 1
                    Label {
                        font.pixelSize: 20
                        font.bold: true
                        text:qsTr("Sex")
                    }


                    ComboBox {
                        id: sex
                        model: ListModel {
                             ListElement{ value: User.Man
                                 text: qsTr("Man") }
                             ListElement{ value: User.Woman
                                 text: qsTr("Women") }
                         }
                        textRole: "text"
                        valueRole: "value"
                        implicitWidth:  400
                        implicitHeight: 40
                        Layout.alignment: Qt.AlignLeft
                        Binding {
                            target: initUserData;
                            property: "sex";
                            value: sex.currentValue
                        }
                    }

                }

                ColumnLayout {
                    spacing: 1
                    Label {
                        font.pixelSize: 20
                        font.bold: true
                        text:qsTr("ID(*)")
                    }
                    TextField {
                        id: idcard
                        text: initUserData.idcard
                        implicitWidth:  400
                        implicitHeight: 40
                        font.pixelSize: 17
                        font.bold: true
                        Binding {
                            target: initUserData;
                            property: "idcard";
                            value: idcard.text
                        }
                        focus: true
                        ToolTip {
                            id:idcardToolTip
                            visible: false
                            text: qsTr("A similarly renamed ID already exists!")
                        }
                    }

                }
                ColumnLayout {
                    spacing: 1
                    Label {
                        font.pixelSize: 20
                        font.bold: true
                        text:qsTr("Surgical Date")
                    }
                    TextField {
                        id:surgicalDate
                        implicitWidth:  400
                        implicitHeight: 40
                        font.pixelSize: 17
                        font.bold: true
                        readOnly: true
                        text: initUserData.birthday.toLocaleDateString(Qt.locale(InterfaceSettings.language))
                        inputMethodHints: Qt.ImhDate

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
                            selectedDate:initUserData.birthday
                            minDate:new Date(new Date().getFullYear() - 100, 1, 1)
                            maxDate:new Date(new Date().getFullYear() + 10, 1, 1)

                            onFinished:(selectedDate)=> {
                                initUserData.birthday =selectedDate;
                                datePicker.close()
                            }
                        }
                    }

                }
                ColumnLayout {
                    spacing: 1
                    Label {
                        font.pixelSize: 20
                        font.bold: true
                        text:qsTr("Age")
                    }
                    function calculateAge(birthDate) {

                    }
                    TextField {
                        id: age
                        readOnly:true
                        implicitWidth:  400
                        implicitHeight: 40
                        font.pixelSize: 17
                        font.bold: true
                        text: format()
                        function format() {
                            var today = new Date();
                            var birthDate = new Date(initUserData.birthday);
                            var age = today.getFullYear() - birthDate.getFullYear();
                            var m = today.getMonth() - birthDate.getMonth();
                            if (m < 0 || (m === 0 && today.getDate() < birthDate.getDate())) {
                              age--;
                            }
                            return Math.max(age, 0);
                        }
                        hoverEnabled:false
                        Layout.alignment: Qt.AlignCenter
                    }
                }

                ColumnLayout {
                    spacing: 1
                    Label {
                        font.pixelSize: 20
                        font.bold: true
                        text:qsTr("Weight(KG)")
                    }
                    TextField {
                        id: weight
                        text: initUserData.weight
                        inputMethodHints: Qt.ImhDigitsOnly
                        validator: IntValidator {}
                        Binding {
                            target: initUserData;
                            property: "weight";
                            value: weight.text
                        }
                        focus: true
                        implicitWidth:  400
                        implicitHeight: 40
                        font.pixelSize: 17
                        font.bold: true
                        Layout.alignment: Qt.AlignLeft
                    }
                }

                ColumnLayout {
                    spacing: 1
                    Label {
                        font.pixelSize: 20
                        font.bold: true
                        text:qsTr("Doctor")
                    }

                    TextField {
                        id: doctor
                        text: initUserData.doctor

                        Binding {
                            target: initUserData;
                            property: "doctor";
                            value: doctor.text
                        }
                        focus: true
                        implicitWidth:  400
                        implicitHeight: 40
                        font.pixelSize: 17
                        font.bold: true
                        Layout.alignment: Qt.AlignLeft
                    }

                }
            }
            // ColumnLayout {
            //     spacing: 1
            //     Layout.fillHeight: true
            //     Layout.fillWidth: true
            //     Label {
            //         font.pixelSize: 20
            //         font.bold: true
            //         text:qsTr("备注")
            //     }

            //     ScrollView {
            //         Layout.fillHeight: true
            //         Layout.fillWidth: true
            //         implicitWidth:  300
            //         implicitHeight: 100

            //         TextArea {
            //             id: description

            //             text: initUserData.description
            //             background:Rectangle {
            //                 color:"#191919"
            //                 radius:3
            //                 border.color: "#5f5f5f"
            //                 border.width: 1
            //             }
            //         }
            //     }
            //     Binding {
            //         target: initUserData;
            //         property: "description";
            //         value: description.text
            //     }

            // }

        }

        Button {
            text:qsTr("Save")
            Layout.fillWidth: true
            implicitHeight: 50
            font.pixelSize: 17
            font.bold: true
             enabled: initUserData.name !== ""  && initUserData.idcard !== "" && initUserData.birthday
             onClicked:  {
                 finished();
             }
        }
        Button {
            text:qsTr("Back")
            Layout.fillWidth: true
            implicitHeight: 50
            font.pixelSize: 17
            font.bold: true
            onClicked: {
                cancel();
            }
        }


    }

}
