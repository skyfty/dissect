import QtQuick

Item {
    id:root
    property var cursorShape:null
    MouseArea{
       id:mouseArea;
       anchors.fill: parent;
       onPositionChanged: (mouse)=>{
           setCursorPos(mouse.x, mouse.y);
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
