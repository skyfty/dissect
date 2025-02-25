import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import "utils.js" as Utils

ComboBox {
    id: control
    model:Utils.SurveyorRegulateList
    displayText: currentText + " mm/mV"


    delegate: MenuItem {
        width: ListView.view.width
        text: modelData + " mm/mV"
        font.weight: control.currentIndex === index ? Font.DemiBold : Font.Normal
        highlighted: control.highlightedIndex === index
        hoverEnabled: control.hoverEnabled
    }
}
