import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts 2.5

ChartView {
    id:root
    property alias max: axisX.max
    property alias ymax: axisY.max
    property alias ymin: axisY.min
    property bool axisVisible:true
    property alias useOpenGL: xSeries.useOpenGL
    property alias tickCount: axisX.tickCount

    antialiasing: true
    legend {
        visible:false
    }
    backgroundColor : "transparent"
    margins {
        bottom:0
        left:0
        right:0
        top:0
    }
    ValueAxis {
        id: axisY
        gridLineColor:"#50808080"
        tickCount:5
        labelsColor:"white"
    }
    ValueAxis {
        id: axisX
        gridLineColor:"#50808080"
        min: 0
        labelsColor:"white"
    }
    LineSeries {
        id:xSeries
        name: "Spline"
        axisX: axisX
        axisY: axisY
        color: "white"
        width:1.0
        capStyle:Qt.RoundCap
    }
}
