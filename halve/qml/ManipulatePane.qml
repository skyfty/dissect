import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import "utils.js" as Utils

Item {
    id:root
    signal pressed(var mouse)
    signal released(var mouse)
    signal positionChanged(var mouse)
    signal entered(var mouse)
    signal exited(var mouse)

    property int type:0
    property int startX: -1
    property int startY: -1
    property int stopX: -1
    property int stopY: -1
    property bool isMouseMoveEnable: false
    property var cursorShape:null
    property var polygonPoints: []
    property alias cursorVisible: customCursor.visible
    property alias pressedButtons: mouseArea.pressedButtons


    function reset() {
        startX = startY = stopX = stopY = 0;
    }

    Canvas{
       id:canvas;
       anchors.fill:parent

       onPaint: {
           var ctx = getContext("2d")

           ctx.lineWidth = 2
           ctx.strokeStyle = "green";
           ctx.clearRect(0,0,width,height)
           if (!isMouseMoveEnable) {
               return;
           }
           stopX = mouseArea.mouseX
           stopY = mouseArea.mouseY

           if (polygonPoints.length > 0) {
               ctx.beginPath()
               for (var i = 0; i < polygonPoints.length; i++) {
                   var point = polygonPoints[i]
                   if (i === 0) {
                       ctx.moveTo(point.x, point.y)
                   } else {
                       ctx.lineTo(point.x, point.y)
                   }
               }
               ctx.moveTo(startX, startY)
               ctx.lineTo(stopX, stopY)
               ctx.stroke()
           }
       }
    }

    MouseArea{
       id:mouseArea;
       anchors.fill: parent;
       onPressed: (mouse)=>{
           setCursorPos(mouse.x, mouse.y);
           startX = mouseX
           startY = mouseY
           if (type != 2 && (mouse.button === Qt.LeftButton)) {
               isMouseMoveEnable = true
               polygonPoints.push({x: startX, y: startY})
           }
           root.pressed(mouse);
       }

       onReleased: (mouse)=>{
            isMouseMoveEnable = false
           stopX = mouseX
           stopY = mouseY
            root.released(mouse);
            canvas.requestPaint() ;
            polygonPoints = [];
           
       }


       onEntered: (mouse)=>{
           root.entered(mouse);
       }

       onExited:  (mouse)=>{
          root.exited(mouse);
      }

       onPositionChanged: (mouse)=>{
            setCursorPos(mouse.x, mouse.y);
            if (type == 1 && startX !== -1 && polygonPoints.length > 0) {
                var lastPos = polygonPoints[polygonPoints.length - 1];
                var distance = Utils.calculateDistance(lastPos.x, lastPos.y, mouse.x, mouse.y);
                if (distance > 10) {
                    polygonPoints.push({x: mouse.x, y: mouse.y})
                }
            }
            

            if (isMouseMoveEnable){
               canvas.requestPaint()   //绘制函数
            }
            root.positionChanged(mouse);
       }

       function setCursorPos(x, y) {
           customCursor.x  = x - customCursor.width / 2
           customCursor.y = y - customCursor.height /2 ;
       }

       cursorShape:root.cursorShape !== null?Qt.BlankCursor:Qt.ArrowCursor
    }

    Item {
        id: customCursor
        height: 25
        width: 25
        Rectangle {
            anchors.fill: parent
            color:"DarkSlateGray"
            radius: 10
            opacity: 0.8
            border.width: 1
            border.color: "white"
        }

        Image {
            anchors.margins: 4
            visible: root.cursorShape !== null
            anchors.fill: parent
            source: root.cursorShape !== null ?root.cursorShape:""
        }
    }


}
