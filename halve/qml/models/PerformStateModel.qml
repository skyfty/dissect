import QtQuick
import Halve


ListModel {
    id:listModel
    ListElement {
        text: qsTr("Modeling")
        icon: "qrc:/assets/images/jianmo.png"
        value:Profile.Reproduce
    }
    ListElement {
        text: qsTr("Mapping")
        icon: "qrc:/assets/images/xiaorong.png"
        value:Profile.Melt
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
