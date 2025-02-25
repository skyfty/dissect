pragma Singleton
import QtCore
import QtQuick 2.0
import Halve

Settings {
    category:"interface"
    property string language: "en"
    property bool fullScreen: false
    property string ecgElectrodeColor: "yellow";
}
