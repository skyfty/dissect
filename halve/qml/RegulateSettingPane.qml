import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtQuick.Dialogs
import "utils.js" as Utils

ColumnLayout {
    id:root
    required property var setting
    GroupBox {
        title: qsTr("ECG")
        Layout.fillWidth: true
        SurveyorComboBox {
            Component.onCompleted: {
                if (setting) {
                    currentIndex =  indexOfValue(setting.ecgSurveyor);
                }
            }
            onActivated: {
                if (setting) {
                    setting.ecgSurveyor = currentValue;
                }
            }
        }
    }

    GroupBox {
        title: qsTr("Body")
        Layout.fillWidth: true
        SurveyorComboBox {
            Component.onCompleted: {
                currentIndex =  indexOfValue(setting.bodySurveyor);
            }
            onActivated: {
                setting.bodySurveyor = currentValue;
            }
        }
    }
}

