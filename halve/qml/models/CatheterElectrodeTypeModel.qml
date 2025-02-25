import QtQuick

import Halve

ListModel {
    id:listModel
    ListElement {
        text: qsTr("None")
        value:Halve.CET_TUBE
    }
    ListElement {
        text: qsTr("Mag")
        value:Halve.CET_MAG
    }
    ListElement {
        text: qsTr("Pant")
        value:Halve.CET_PANT
    }
    ListElement {
        text: qsTr("Assist")
        value:Halve.CET_ASSIST
    }
    ListElement {
        text: qsTr("Handle")
        value:Halve.CET_HANDLE
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
