import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts


RowLayout {
    id:root
    required property var filterOptions
    GroupBox {
        title: qsTr("ECG")
        Layout.fillWidth: true
        FilterForm {
            anchors.fill: parent
            mode:filterOptions.ecg

        }
    }

    GroupBox {
        title: qsTr("导管双极")
        Layout.fillWidth: true
        FilterForm {
            mode:filterOptions.catheterDouble
            anchors.fill: parent

        }
    }
    GroupBox {
        title: qsTr("导管单级")
        Layout.fillWidth: true
        FilterForm {
            mode:filterOptions.catheterSingle
            anchors.fill: parent

        }
    }
}

