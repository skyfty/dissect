import QtQuick

RulerPane {
    id:root
    property int lastWidth:0
    onWidthChanged: {
        reset();
        if (root.lastWidth !== 0) {
            var ss = root.width / root.lastWidth ;
            for(var i = 0; i < lines.length; ++i) {
                lines[i].startX *= ss;
                lines[i].stopX *= ss;
            }
            repaint();
        }
        root.lastWidth = width;
    }
}
