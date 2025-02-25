import QtQuick
import Halve

ListModel {
    ListElement {
     text: qsTr("LAT")
     value:Halve.Lat
    }
    ListElement {
     text: qsTr("Voltage")
     value:Halve.Voltage
    }

}
