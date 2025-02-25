import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: root
    signal finished(var selectedDate);
    property date selectedDate: new Date()
    property date minDate: new Date(1970, 1, 1)
    property date maxDate: new Date()

    implicitWidth: layout.width
    implicitHeight: layout.height
    color: "transparent"

    function range(start : int, end : int) : list<int> {
        return new Array(end - start + 1).fill().map((_, idx) => start + idx)
    }

    ColumnLayout {
        id: layout
        Rectangle {
            id: controls
            color: "transparent"
            implicitHeight: 25
            Layout.fillWidth: true

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 8
                spacing: 1
                TextField {
                    id:yearField
                    readOnly: true
                    color: "#ffffff"
                    font.pixelSize: 13
                    verticalAlignment: Text.AlignVCenter
                    topPadding: 4
                    leftPadding: 4
                    rightPadding: 4
                    bottomPadding: 4
                    Layout.fillHeight: true
                    Layout.preferredWidth: 100
                    text: monthGrid.year
                    background: Rectangle {
                        clip: true
                        radius: 3
                        color:"#282828"

                    }
                    ColorImage {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        color: "white"
                        source: "qrc:/qt-project.org/imports/QtQuick/Controls/Fusion/images/arrow.png"
                        width: 20
                        fillMode: Image.Pad
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            yearDialog.open()
                        }
                    }

                    Dialog {
                        id: yearDialog
                        modal:true
                        width: 270; height: 250

                        ColumnLayout {
                            anchors.fill: parent

                            GridView {
                                id: yearGrid
                                Layout.fillHeight: true
                                Layout.fillWidth: true

                                clip: true
                                cellWidth: 50
                                cellHeight: 30
                                model: range(minDate.getFullYear(), maxDate.getFullYear())
                                delegate: Item {
                                    width: yearGrid.cellWidth
                                    height: yearGrid.cellHeight
                                    Text {
                                        anchors.centerIn: parent
                                        text: modelData;
                                        font.pixelSize: 16
                                        color:"white"
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            yearGrid.currentIndex = index
                                        }
                                    }
                                }

                                focus: true
                                highlightFollowsCurrentItem: false
                                highlight: Rectangle {
                                    width: yearGrid.cellWidth
                                    height: yearGrid.cellHeight
                                    color: "#0078d4"
                                    radius: 2
                                    x: yearGrid.currentItem.x
                                    y: yearGrid.currentItem.y
                                }
                                Component.onCompleted: yearGrid.currentIndex = yearGrid.model.indexOf(monthGrid.year)
                            }

                            Button {
                                text:qsTr("Ok")
                                Layout.fillWidth: true
                                Layout.preferredHeight: 30
                                onClicked: {
                                    yearField.text = monthGrid.year = yearGrid.model[yearGrid.currentIndex];
                                    yearDialog.close();
                                }
                            }
                        }
                    }

                }

                ComboBox {
                    id: monthButton
                    Layout.fillHeight: true
                    Layout.preferredWidth: 100
                    textRole: "text"
                    valueRole: "value"
                    model: ListModel {
                         ListElement {
                             text: qsTr("January")
                             value:Calendar.January
                         }

                         ListElement {
                             text: qsTr("February")
                             value:Calendar.February
                         }
                         ListElement {
                             text: qsTr("March")
                             value:Calendar.March
                         }
                         ListElement {
                             text: qsTr("April")
                             value:Calendar.April
                         }
                         ListElement {
                             text: qsTr("May")
                             value:Calendar.May
                         }
                         ListElement {
                             text: qsTr("June")
                             value:Calendar.June
                         }
                         ListElement {
                             text: qsTr("July")
                             value:Calendar.July
                         }
                         ListElement {
                             text: qsTr("August")
                             value:Calendar.August
                         }
                         ListElement {
                             text: qsTr("September")
                             value:Calendar.September
                         }
                         ListElement {
                             text: qsTr("October")
                             value:Calendar.October
                         }
                         ListElement {
                             text: qsTr("November")
                             value:Calendar.November
                         }
                         ListElement {
                             text: qsTr("December")
                             value:Calendar.December
                         }
                     }

                    onActivated: {
                        monthGrid.month = currentValue
                    }

                    Component.onCompleted: currentIndex = indexOfValue(monthGrid.month)

                }

                Item {
                    Layout.fillWidth: true

                }

                Button {
                    text:"确定"
                    Layout.fillHeight: true
                    onClicked: {
                        finished(selectedDate);
                    }
                }
            }
        }

        Rectangle {
            color: "transparent"
            implicitHeight: 5
            Layout.fillWidth: true
        }

        Rectangle {
            color: "#0078d4"
            implicitHeight: 1
            Layout.fillWidth: true
        }

        DayOfWeekRow {
            locale: Qt.locale(InterfaceSettings.language)
            spacing:21
            Layout.leftMargin: 10
            delegate: Text {
                color:"white"
                font.pixelSize: 11
                text: shortName
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                required property string shortName
            }
        }
        MonthGrid {
            id: monthGrid
            year:selectedDate.getFullYear()
            month: selectedDate.getMonth()
            locale: Qt.locale(InterfaceSettings.language)
            Layout.fillWidth: true
            delegate: Rectangle {
                required property var model
                color: 'transparent'
                radius: 20
                border.width: 1
                border.color: isCurrentDate() ? "#0078d4" : "transparent"

                width: 37
                height: 37

                Text {
                    anchors.centerIn: parent
                    text: monthGrid.locale.toString(model.date, "d")
                    color: model.month === monthGrid.month ? 'white' : 'gray'
                }

                function isCurrentDate() {
                    return root.selectedDate.getFullYear() === model.year &&  root.selectedDate.getMonth() === model.month &&  root.selectedDate.getDate() === model.day;
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        cursorShape = Qt.PointingHandCursor
                        color = "#0078d4"
                    }
                    onExited: {
                        cursorShape = Qt.ArrowCursor
                        color = 'transparent'
                    }
                    onClicked: {
                        yearField.text = model.year;
                        monthButton.currentIndex = monthButton.indexOfValue(monthGrid.month)
                        root.selectedDate = new Date(model.year,model.month,model.day)
                    }
                }


            }
        }

    }

}
