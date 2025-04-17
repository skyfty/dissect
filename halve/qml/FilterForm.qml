import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window
import "models"


ColumnLayout {
    id:root
    required property var mode
    spacing:9

    ColumnLayout {
        Label {
            text: "高通滤波"
            Layout.fillWidth: true
        }
        ComboBox {
            implicitHeight: 25
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            textRole: "text"
            valueRole: "value"
            model: HighPassModel{}
            Component.onCompleted: currentIndex = indexOfValue(mode.highPass)
            onActivated: {
                mode.highPass = currentValue
            }
        }
    }
    ColumnLayout {
        Label {
            text: "低通滤波 "
            Layout.fillWidth: true
        }
        ComboBox {
            implicitHeight: 25
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            textRole: "text"
            valueRole: "value"
            model: LowPassModel{}
            Component.onCompleted: currentIndex = indexOfValue(mode.lowPass)
            onActivated:mode.lowPass = currentValue
        }
    }

    CheckBox {
        text:"噪音滤波器"
        checked: mode.noise
        onClicked: mode.noise = checked
    }
    CheckBox {
        text:"磁场滤波器"
        checked: mode.magnetic
        onClicked: mode.magnetic = checked
    }
    CheckBox {
        text:"10Hz陷波器"
        checked: mode.notch10Hz
        onClicked: mode.notch10Hz = checked
    }
    CheckBox {
        text:"时域处理"
        checked: mode.timeSeriesProcess
        onClicked: mode.timeSeriesProcess = checked
    }
}

