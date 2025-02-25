
import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve 1.0

Item {
    id:root
    required property var profile
    property var breathOptions:profile.breathOptions
    required property var breathSurvey

    Connections {
        target: breathSurvey
        function onPantZPositionChanged() {
            var ratioInWidth= (root.breathOptions.breathePeak - root.breathOptions.breatheValley) / (handleLineWrap.height);
            var ypos = handleLineWrap.height - (root.breathSurvey.pantZPosition - breathOptions.breatheValley)  / ratioInWidth;
            handleLine.y = ypos;
        }
    }

    BreathChartListModel {
        id:breathChartListModel
        profile:root.profile
    }

    Item {
        anchors.fill: parent
        RowLayout {
            anchors.fill: parent
            Repeater {
                model: breathChartListModel
                PantChart {
                    required property int index
                    required property var axisyMinValue
                    required property var axisyMaxValue
                    required property var axisxMaxValue
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    max:axisxMaxValue
                    ymax:axisyMaxValue
                    ymin:axisyMinValue
                    Component.onCompleted: {
                        breathChartListModel.setBreathSeries(index, this.series(0))
                    }
                    Component.onDestruction: {
                        breathChartListModel.setBreathSeries(index, null)
                    }
                }
            }
        }

        Item {
            id:handleLineWrap
            anchors.fill: parent
            anchors.topMargin: 8
            anchors.bottomMargin: 22
            anchors.rightMargin: 12
            anchors.leftMargin: 22
            clip: true

            Rectangle {
                id:handleLine
                width:parent.width
                anchors.horizontalCenter:parent.horizontalCenter
                height: 1
                color: "lightgreen"
            }
        }

    }

}
