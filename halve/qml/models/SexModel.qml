import QtQuick
import Halve

ListModel {
    id:listModel
    ListElement {
        text: qsTr("Woman")
        value:User.Woman
    }
    ListElement {
        text: qsTr("Man")
        value:User.Man
    }
    function getTextByValue(v) {
        for (var i = 0; i < listModel.count; i++) {
            if (listModel.get(i).value === (v)) {
                return listModel.get(i).text;
            }
        }
        return null;
    }
}
