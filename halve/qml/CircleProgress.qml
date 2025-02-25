import QtQuick
import QtQuick.Window
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id:rect
    width: 300
    height: width
    radius: width/2
    property int value:0  //取值0-100
    property string title
    antialiasing: true
    anchors.centerIn: parent
    color: "gray"
    ColumnLayout {
        anchors.centerIn: parent
        Text {
            id: ratio
            Layout.alignment: Qt.AlignHCenter
            text: String("%1%").arg(rect.value)
            font{
                pointSize: 30
                family: "黑体"
            }
            color: "white"
        }
        Label {
            Layout.alignment: Qt.AlignHCenter
            text:title
            font{
                pointSize: 14
                family: "黑体"
            }
            color: "white"
        }
    }

    onValueChanged: {
        canvas.requestPaint()
    }

    Canvas{
        id:canvas
        antialiasing: true
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d")

            //绘图之前清除画布
            ctx.clearRect(0,0,width,height)

            ctx.strokeStyle = "aquamarine"
            ctx.lineWidth = 12
            ctx.beginPath()
            ctx.arc(rect.width/2,rect.height/2,rect.width/2-ctx.lineWidth,-Math.PI/2,-Math.PI/2+rect.value/100*2*Math.PI )
            ctx.stroke()
        }
    }
}

