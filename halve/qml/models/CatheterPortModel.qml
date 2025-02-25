import QtQuick

ListModel {
    id:listModel
    ListElement {
        text: qsTr("Back reference")
        value:0
        enabled: false
    }
    ListElement {
        text: qsTr("Prethoracic reference")
        value:1
        enabled: false
    } // 禁止选择
    ListElement {
        text: qsTr("Port 3")
        value:2
        enabled: true
    }
    ListElement {
        text: qsTr("Port 4")
        value:3
        enabled: true
    }
    ListElement {
        text: qsTr("Port 5")
        value:4
        enabled: true
    }
    ListElement {
        text: qsTr("Port 6")
        value:5
        enabled: true
    }
    ListElement {
        text: qsTr("Port 7")
        value:6
        enabled: true
    }
    ListElement {
        text: qsTr("Port 8")
        value:7
        enabled: true
    }
    function getTextByValue(v) {
        for (var i = 0; i < listModel.count; i++) {
            if (listModel.get(i).value  === v) {
                return listModel.get(i).text;
            }
        }
        return null;
    }
}
