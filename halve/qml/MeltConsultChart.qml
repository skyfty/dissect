import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtCharts 2.5
import QtQuick.Dialogs
import "utils.js" as Utils
Item {
    id:root
    required property var dyestuff
    required property string name
    required property var type
    required property var setting
    property alias chart:consultChart

    RowLayout {
        anchors.fill: parent
        spacing: 0
        Control {
            id:nameControl
            Layout.maximumWidth: AppSettings.channelChartListLabelWidth
            Layout.minimumWidth: AppSettings.channelChartListLabelWidth
            Layout.fillHeight: true
            Layout.margins:0
            Label {
                verticalAlignment : Text.AlignVCenter
                horizontalAlignment:Text.AlignRight
                anchors.verticalCenter: parent.verticalCenter
                rightPadding: 10
                anchors.fill: parent
                color:root.dyestuff
                text: root.name
                font.pixelSize: 13
            }
            ColorCarp{
                id:meltElectrodeColorPicker
                color:root.dyestuff
                visible: nameControl.hovered
                width:15
                height: 15
                x:10
                radius: 5
                anchors.verticalCenter: parent.verticalCenter
                onAccepted: {
                    root.dyestuff = meltElectrodeColorPicker.color
                }
            }
        }

        ChannelChart {
            id:consultChart
            Layout.fillWidth: true
            Layout.fillHeight:true
            property var basis:root.type === Halve.ECG?AppSettings.ecgBasis:AppSettings.basis
            property int basisHeight:basis/2

            function resizeBasisHeight() {
                var j = consultChart.height/surveyor.height;
                var j2 = basis/2;
                consultChart.basisHeight = j * j2;
            }
            onHeightChanged:resizeBasisHeight()
            color:root.dyestuff
            useOpenGL: true
            max:root.setting.max
            ymin: -basisHeight
            ymax: basisHeight
            SurveyorLine {
                id:surveyor
                y:0
                x:0
                idx:AppSettings.defaultSurveyor
                anchors.verticalCenter: parent.verticalCenter
                chartHeight:consultChart.height
                onHeightChanged:parent.resizeBasisHeight()
                value: root.type === ElectrodeNode.ECG ?root.setting.ecgSurveyor:(root.type !== ElectrodeNode.PANT?root.setting.bodySurveyor:AppSettings.defaultSurveyor)
            }

            Loader {
                active: root.setting.rulerType !== 0
                anchors.fill: parent
                asynchronous: true
                visible: status == Loader.Ready
                sourceComponent: MeltRulerPane {
                    anchors.fill: parent
                    type:root.setting.rulerType
                    latBasis: root.setting.max
                    regulate:surveyor.idx
                }
            }
        }
    }

}
