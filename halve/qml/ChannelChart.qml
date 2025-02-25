import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts 2.5

ChartView {
    id:root
    property alias max: axisX.max
    property alias color: splineSeries.color
    property alias useOpenGL: splineSeries.useOpenGL
    property alias ymax: axisY.max
    property alias ymin: axisY.min
    property bool axisVisible:false
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
        labelsVisible: false
        gridLineColor:"#50808080"
        titleVisible:false
        lineVisible:false
        visible:root.axisVisible
        shadesVisible:false
        tickCount:5
        tickType:ValueAxis.TicksFixed
    }
    ValueAxis {
        id: axisX
        labelsVisible: false
        lineVisible:false
        shadesVisible:false
        titleVisible:false
        gridLineColor:"#50808080"
        visible:root.axisVisible
        min: 0
        tickType:ValueAxis.TicksFixed
    }
    LineSeries {
        id:splineSeries
        name: "Spline"
        axisX: axisX
        axisY: axisY
        color: "white"
        width:1.0
        capStyle:Qt.RoundCap
    }
}
