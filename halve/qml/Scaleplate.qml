import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property int minValue: 0 // 标尺最小值
    property int maxValue: 100 // 标尺最大值
    property int step: 10 // 标尺间隔
    property int minorTicks: 9 // 标尺间隔

    property string unit: "ms"
    property bool vertical:false
    property bool mirror:false
    property bool showLable:true

    function requestPaint() {
        canvas.requestPaint();
    }

    onMaxValueChanged: {
        canvas.requestPaint();
    }
    onMinValueChanged: {
        canvas.requestPaint();
    }
    onStepChanged: {
        canvas.requestPaint();
    }

    Canvas {
        id:canvas
        anchors.fill: parent

        function drawMainLine(ctx,x, i) {
            var txt = i;
            if (root.vertical) {
                ctx.beginPath();
                ctx.moveTo(0, x );
                ctx.lineTo(root.width, x);
                ctx.stroke();
                if (root.showLable) {
                    if (i === 0) {
                        txt +=" " + root.unit
                    }
                    if (root.mirror) {

                    } else {
                        ctx.fillText(txt, root.width+4, x);
                    }

                }
            } else {
                ctx.beginPath();
                ctx.moveTo(x, 0 );
                ctx.lineTo(x, root.height);
                ctx.stroke();

                 if (root.showLable) {
                    if (i === 0) {
                        txt +=" " + root.unit
                    }
                    ctx.fillText(txt, x+4, root.height);
                 }
            }
        }

        function drawTickLine(ctx,minorStepWidth,minorTicks,i) {
            for (var j = 1; j <= minorTicks; j++) {
                var x = (i - root.minValue) / (root.maxValue - root.minValue) * (root.vertical?root.height:root.width) + j * minorStepWidth;
                ctx.beginPath();
                if (root.vertical) {
                    if (root.mirror) {
                        ctx.moveTo(5, x);
                        ctx.lineTo(root.width, x);
                    } else {
                        ctx.moveTo(0, x);
                        ctx.lineTo(root.width - 5, x);
                    }

                } else {
                    ctx.moveTo(x, 0);
                    ctx.lineTo(x, root.height - 10);
                }

                ctx.stroke();

            }
        }

        onPaint: {
            var ctx = getContext("2d");

            var stepWidth = 0;
            if (root.vertical) {
                stepWidth = root.height / ((root.maxValue - root.minValue) / root.step);
            } else {
                stepWidth = root.width / ((root.maxValue - root.minValue) / root.step);
            }


            // 绘制标尺背景
            ctx.fillStyle = "#000000"; // 设置背景颜色为黑色
            ctx.fillRect(0, 0, root.width, root.height);

            // 设置刻度线和数字颜色为白色
            ctx.strokeStyle = "#FFFFFF";
            ctx.fillStyle = "#FFFFFF";
            ctx.font = "10px sans-serif"; // 设置数字字体大小和样式

            // 绘制主要刻度线和数字

            var i = 0;
            while(true) {
                var x = (i - root.minValue) / (root.maxValue - root.minValue) * (root.vertical?root.height:root.width);
                if (i <= root.minValue) {
                    drawMainLine(ctx, x, root.minValue);
                    break;
                } else {
                    drawMainLine(ctx, x, i);
                    i -= root.step
                }
            }

            i = 0;
            while(true) {
                var x = (i - root.minValue) / (root.maxValue - root.minValue) * (root.vertical?root.height:root.width);
                if (i >= root.maxValue) {
                    drawMainLine(ctx, x, root.maxValue);
                    break;
                } else {
                    drawMainLine(ctx, x, i);
                    i += root.step
                }
            }

            var minorStepWidth = stepWidth / (root.minorTicks + 1);
            var minorTicks = root.minorTicks; // 次要刻度的数量

            i = 0;
            while(true) {
                if (i < root.minValue) {
                    var minorSpance = minorTicks - Math.floor(minorTicks * (Math.abs(i - root.minValue) / root.step)) - 1;
                    drawTickLine(ctx, minorStepWidth,minorSpance, root.minValue);
                    break;
                } else {
                    drawTickLine(ctx, minorStepWidth,minorTicks, i);
                    i -= root.step
                }
            }


            i = 0;
            while(true) {
                if (i > root.maxValue) {
                    var minorSpance = minorTicks - Math.floor(minorTicks * (Math.abs(root.maxValue - i) / root.step)) - 1;
                    drawTickLine(ctx, minorStepWidth,minorSpance, root.maxValue);
                    break;
                } else {
                    i += root.step
                    drawTickLine(ctx, minorStepWidth,minorTicks, i);
                }
            }
        }
    }
}
