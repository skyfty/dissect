import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import "utils.js" as Utils

Item {
    id:root

    property int type:0
    property int startX: -1
    property int startY: -1
    property int stopX: -1
    property int stopY: -1
    property var cursorShape:null
    property real dpi: Screen.pixelDensity
    property real regulate:0
    property var latBasis
    property var lines:[]
    property int lastHeight:0
    property var regulateList: Utils.SurveyorRegulateList
    property var ratioInWidth:latBasis / root.width



    function reset() {
        startX = startY = stopX = stopY = -1;
    }

    function repaint() {
        canvas.requestPaint();
    }

    onHeightChanged: {
        reset();
        if (root.lastHeight !== 0) {
            var ss = root.height / root.lastHeight ;
            for(var i = 0; i < lines.length; ++i) {
                lines[i].startY *= ss;
                lines[i].stopY *= ss;
            }
            canvas.requestPaint();
        }
        root.lastHeight = root.height;
    }

    onRegulateChanged: {
        canvas.requestPaint();
    }

    Canvas{
       id:canvas;
       anchors.fill:parent

       function drawLine(ctx, t, x1, y1, x2, y2) {
           var distane = Math.sqrt(Math.pow((x2 - x1),2) + Math.pow((y2 - y1), 2));
            ctx.strokeStyle = (t ===1?"orange":"green");
            ctx.beginPath()
            ctx.moveTo(x1,y1)
            ctx.lineTo(x2,y2)
            ctx.stroke()

            if (distane > 0) {
                switch(t) {
                case 1: {
                    var dx = x2 + 2;
                    if (dx > width) {
                        dx = width - 60;
                    }
                    var dy = y2 + 4;
                    var latValue = Math.floor((distane * ratioInWidth) / AppSettings.samplingRate * 1000)
                    ctx.fillText(latValue + " ms",dx, dy)
                    break;
                }
                case 2: {
                    var dx = x2 + 2;
                    var dy = y2 ;
                    if (dy === 0) {
                        dy = 10;
                    }
                    distane = (distane / root.dpi);
                    ctx.fillText((distane / regulateList[regulate]).toFixed(2) + " mV",dx, dy)
                    break;
                }
                }
            }
       }

       onPaint: {
           var ctx = getContext("2d")
           ctx.clearRect(0,0,width,height);
           ctx.lineWidth = 2
           ctx.fillStyle = "white";
           ctx.font = "bold 15px sans-serif";
           if (root.type ===1) {
                stopX = mouseArea.mouseX
               if (stopX < 0 ) {
                   stopX = 0;
               } else if (stopX > width) {
                   stopX = width;
               }
                stopY = startY;
           } else {
               stopX = startX;
               stopY = mouseArea.mouseY
               if (stopY < 0 ) {
                   stopY = 0;
               } else if (stopY > height) {
                   stopY = height;
               }
           }
           if (startX !== -1 && startY !== -1) {
               drawLine(ctx, type, startX,startY,stopX, stopY);
           }

           for(var i = 0; i < lines.length; ++i) {
               drawLine(ctx, lines[i].type, lines[i].startX,lines[i].startY,lines[i].stopX, lines[i].stopY);
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
       }

       onReleased: (mouse)=>{
           if (root.type ===1) {
                stopX = mouseX;
               if (stopX < 0 ) {
                   stopX = 0;
               } else if (stopX > width) {
                   stopX = width;
               }
                stopY = startY;
           } else {
               stopX = startX;
               stopY = mouseY;
               if (stopY < 0 ) {
                   stopY = 0;
               } else if (stopY > height) {
                   stopY = height;
               }
           }
           root.lines.push({"type":root.type, "startX":startX, "startY":startY, "stopX":stopX, "stopY":stopY})
       }

       onPositionChanged: (mouse)=>{
          if (root.type ===1) {
             setCursorPos(mouse.x, startY);
          } else {
             setCursorPos(startX, mouse.y);
          }
        canvas.requestPaint()   //绘制函数
       }

       function setCursorPos(x, y) {
           customCursor.x  = x - customCursor.width / 2
           customCursor.y = y - customCursor.height /2 ;
       }

       cursorShape:root.cursorShape !== null?Qt.BlankCursor:Qt.ArrowCursor
    }

    Image {
        id: customCursor
        visible: root.cursorShape !== null
        height:20
        width: 20
        source: root.cursorShape !== null ?root.cursorShape:""
    }

}
