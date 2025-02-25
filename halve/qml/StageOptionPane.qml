import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve

ColumnLayout {
    id:root
    required property var options
    spacing: 7

    ColumnLayout {
        Layout.fillWidth: true
        Label {text:qsTr("Representation")}
        ComboBox {
            Layout.fillWidth: true
            textRole: "text"
            valueRole: "value"
            model: ListModel{
                ListElement{ value: Halve.SURFACE
                    text: qsTr("Surface") }
                ListElement{ value: Halve.POINT_CLOUD
                    text: qsTr("Points") }
                ListElement{ value: Halve.WIREFRAME
                    text: qsTr("Wireframe") }
                ListElement{ value: Halve.SURFACE_WITH_EDGE
                    text: qsTr("Surface With Edge") }

            }
            onActivated:root.options.representation = currentValue
            Component.onCompleted:currentIndex = indexOfValue(root.options.representation)
        }
    }

    GroupBox {
        title: qsTr("Stage")
        Layout.fillWidth: true
        GridLayout {
            columns: 2
            CheckBox {
                id:showAxesCheckbox
                checked: root.options.showAxes
                text:qsTr("Data Axes Grid")
                onClicked: root.options.showAxes = checked
            }
            CheckBox {
                id:useAxisOriginCheckbox
                checked: root.options.useAxisOrigin
                text:qsTr("Use axis origin")
                enabled: showAxesCheckbox.checked
                onClicked: root.options.useAxisOrigin = checked
            }
            CheckBox {
                id:showInnerGridlinesCheckbox
                checked: root.options.showInnerGridlines
                text:qsTr("Show inner grid lines")
                enabled: showAxesCheckbox.checked && !useAxisOriginCheckbox.checked
                onClicked: root.options.showInnerGridlines = checked
            }
            CheckBox {
                id:showOrientationCheckbox
                checked:  root.options.showOrientation
                text:qsTr("Orientation Axes Visibility")
                onClicked: root.options.showOrientation = checked
            }
            CheckBox {
                id:scalarBarCheckbox
                checked:  root.options.scalarBar
                text:qsTr("Scalar Bar Visibility")
                onClicked: root.options.scalarBar = checked
            }

            CheckBox {
                id:backfaceCullingCheckbox
                text: qsTr("Backface culling")
                checked:  root.options.backfaceCulling
                onClicked: root.options.backfaceCulling = checked
            }

            CheckBox {
                id:showMeshCheckbox
                text: qsTr("Show Mesh")
                checked:  root.options.showMesh
                onClicked: root.options.showMesh = checked
            }
        }
    }


    GroupBox {
        title: qsTr("Catheter")
        Layout.fillWidth: true
        GridLayout {
            columns: 2
            CheckBox {
                id:showPantCheckbox
                text: qsTr("Show reference")
                checked:  root.options.showPant
                onClicked: root.options.showPant = checked
            }
            CheckBox {
                id:showCatheterLabelCheckbox
                text: qsTr("Show Catheter Label")
                checked:  root.options.showCatheterLabel
                onClicked: root.options.showCatheterLabel = checked
            }
        }
    }


}
