import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve
import QtQuick.Dialogs
import QtQuick3D
import QtQuick3D.Effects
import QtQuick3D.Helpers
import "models"
import "utils.js" as Utils
Item {
    id: root
    required property var profile
    required property var obscurity
    required property var combined
    required property var channel
    required property var reseauListModel
    required property var options
    property var currentReseau : root.profile.currentReseau
    property alias stageScalar:stage.stageScalar
    property alias scalarModel:scalarBar.model
    property alias azimuthModel:azimuthBar.model

    Connections {
        target: root.profile
        function onDissolved() {
            root.options.showDissolvePoints = true;
        }
    }

    Component {
        id:dialogSliceSaveComponent
        Dialog {
            id: dialogSliceSave
            title: qsTr("Save the section")
            modal: true
            anchors.centerIn: parent
            width:300
            height: 200
            SurfaceForm{
                id:surfaceForm
                anchors.margins: 0
                anchors.fill: parent
                onAccepted: {
                    var name = surfaceForm.name.text.trim();
                    if (root.reseauListModel.indexOfName(name) === -1){
                        stage.saveMesh(name, surfaceForm.color);
                        dialogSliceSave.close();
                    } else{
                        Utils.showTip(surfaceForm.name,qsTr("A similarly renamed Name already exists!"));

                    }
                }
                onClosed: dialogSliceSave.close();
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing:0
        Rectangle {
            Layout.fillWidth: true
            Rectangle {
                anchors.fill: parent
                color:"black"
                opacity: 0.5
            }
            color: "transparent"
            height: 36
            clip: true
            RowLayout {
                id:toolBar
                anchors.fill: parent
                anchors.margins: 3
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 1

                ButtonGroup {
                    id:pointerButtonGroup
                    exclusive: true
                }
                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon {
                        source: "qrc:/assets/images/pointer.png"
                        height: 19
                        width:19
                    }
                    ButtonGroup.group: pointerButtonGroup
                    action: Action {
                        checkable: true
                        shortcut: "S"
                        checked: stage.approach === Stage.Drag
                        onTriggered:stage.approach = Stage.Drag
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Drag mode")
                }

                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon {
                        source: "qrc:/assets/images/pointer_add.png"
                        height: 19
                        width:19
                    }
                    ButtonGroup.group: pointerButtonGroup
                    action: Action {
                        enabled: root.currentReseau.apparent && root.currentReseau.pointNumber > 0
                        shortcut: "A"
                        checked: stage.approach ===Stage.Select
                        onTriggered: {
                            if (stage.approach !== Stage.Select ) {
                                stage.approach = Stage.Select
                            } else {
                                stage.approach =Stage.Drag
                            }
                        }

                        onCheckedChanged: {
                            manipulatePane.reset();
                            stage.cleanSelectArea();
                        }
                    }

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Select the area")
                }


                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon {
                        source: stage.woundMode === Halve.WM_ABRADE?"qrc:/assets/images/abrade.svg":"qrc:/assets/images/pierce.svg"
                        height: 19
                        width:19
                    }
                    action: Action {
                        enabled:root.currentReseau.apparent && root.currentReseau.pointNumber > 0
                        checked: stage.approach === Stage.Wound
                        onTriggered: {
                            if (stage.approach !== Stage.Wound ) {
                                stage.approach =Stage.Wound
                            } else {
                                stage.approach = Stage.Drag
                            }
                        }
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: stage.woundMode === Halve.WM_ABRADE?qsTr("Penetration erase"):qsTr("Wipe off the surface")

                    TapHandler {
                        acceptedButtons: Qt.RightButton
                        enabled: root.currentReseau.apparent && root.currentReseau.pointNumber > 0
                        onTapped: (eventPoint, button)=> {
                           if (button === Qt.RightButton) {
                                woundModeMenu.open()
                           }
                        }
                     }

                    Menu {
                        id: woundModeMenu
                        y:parent.height
                        x:0
                        contentItem:Item {
                            anchors.margins: 3
                            ColumnLayout{
                                anchors.fill: parent
                                ToolButton {
                                    text: qsTr("Penetration erase")
                                    icon {
                                        source: "qrc:/assets/images/abrade.svg"
                                        height: 19
                                        width:19
                                    }
                                    action: Action {
                                        enabled:root.currentReseau.apparent && root.currentReseau.pointNumber > 0
                                        checked:  stage.approach === Stage.Wound && stage.woundMode === Halve.WM_ABRADE
                                        onTriggered: {
                                            stage.approach =Stage.Wound
                                            stage.woundMode = Halve.WM_ABRADE;
                                        }
                                    }
                                }

                                ToolButton {
                                    text: qsTr("Wipe off the surface")
                                    icon {
                                        source: "qrc:/assets/images/pierce.svg"
                                        height: 19
                                        width:19
                                    }
                                    action: Action {
                                        enabled: root.currentReseau.apparent && root.currentReseau.pointNumber > 0
                                        checked:  stage.approach === Stage.Wound && stage.woundMode === Halve.WM_PIERCE;
                                        onTriggered: {
                                            stage.approach =Stage.Wound
                                            stage.woundMode = Halve.WM_PIERCE;
                                        }
                                    }
                                }
                            }
                        }

                    }
                }


                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon {
                        source: "qrc:/assets/images/erase.png"
                        height: 19
                        width:19
                    }
                    action: Action {
                        shortcut: "delete"
                        enabled: stage.isSelected
                        onTriggered: {
                            meshDeleteConfirmDialog.open();
                        }
                    }

                    ConfirmDialog {
                        id: meshDeleteConfirmDialog
                        text: qsTr("Are you sure to delete this?")
                        onAccepted: {
                            stage.deleteSelectArea();
                        }
                    }

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Delete")
                }



                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon {
                        source: "qrc:/assets/images/undo.svg"
                        height: 19
                        width:19
                    }
                    onClicked: obscurity.unwound()
                    enabled: obscurity.abradeCount > 0
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Undo")
                }

                ToolSeparator {}

                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon {
                        source: "qrc:/assets/images/slice_save.png"
                        height: 19
                        width:19
                    }
                    action: Action {
                        shortcut: "Ctrl+S"
                        enabled: stage.isSelected
                        onTriggered: {
                            var dialogObject = dialogSliceSaveComponent.createObject(root);
                            dialogObject.closed.connect(function() {
                               dialogObject.destroy()
                           });
                            dialogObject.open();
                        }
                    }

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Save the section")

                }

                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon {
                        source: "qrc:/assets/images/dissolve.svg"
                        height: 19
                        width:19
                    }
                    action: Action {
                        checkable: true
                        enabled:root.currentReseau.apparent && root.currentReseau.pointNumber > 0
                        onTriggered: {
                            stage.approach = checked?Stage.Dissolve:Stage.Drag
                        }
                        checked: stage.approach === Stage.Dissolve
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: "Peep"
                }

                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon {
                        source: "qrc:/assets/images/ruler.svg"
                        height: 19
                        width:19
                    }
                    enabled: stage.approach === Stage.Drag
                    onClicked:stage.gauge = checked
                    checked: stage.gauge
                    checkable: true
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Ruler")
                }
                ToolSeparator {}
                Item{
                    Layout.fillWidth: true
                }

                ToolButton {
                    Layout.alignment: Qt.AlignRight|Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/camera_reset.png"
                        height: 19
                        width:19
                    }
                    action: Action {
                        onTriggered: {
                            stage.resetCameraAzimuth()
                        }
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Reset to center")
                }
                AzimuthBar {
                    id:azimuthBar
                    resetEnabled:stage.azimuthName === ""
                    Layout.alignment: Qt.AlignRight|Qt.AlignVCenter
                    onChanged: (name)=>{
                        stage.setCameraAzimuth(name)
                    }
                    onAdded:(name)=>{
                        stage.azimuthName = name;
                        azimuthBar.model.add(name, stage.cameraInfo);
                    }

                    onRested: {
                        orientation.resetAP();
                    }
                }
            }

        }

        Stage {
            id: stage
            clip: true
            Layout.fillHeight: true
            Layout.fillWidth: true
            profile:root.profile
            options:root.options
            combined:root.combined
            obscurity:root.obscurity
            mappingSetting:root.profile.mappingSetting

            Item {
                anchors.fill: parent

                ManipulatePane {
                    id:manipulatePane
                    anchors.fill: parent
                    states:  [
                        State {
                            name: Stage.Wound
                            PropertyChanges {
                                target: manipulatePane
                                cursorShape:null
                                type:2
                                visible:true
                                cursorVisible:false
                            }
                        },
                        State {
                            name: Stage.Select
                            PropertyChanges {
                                target: manipulatePane
                                cursorShape:null
                                type:1
                                visible:true
                                cursorVisible:false
                            }
                        },
                        State {
                            name: Stage.Drag
                            PropertyChanges {
                                target: manipulatePane
                                cursorShape:null
                                type:0
                                visible:false
                                cursorVisible:false
                            }
                        },
                        State {
                            name: Stage.Dissolve
                            PropertyChanges {
                                target: manipulatePane
                                cursorShape:null
                                type:0
                                visible:false
                                cursorVisible:false
                            }
                        }
                    ]
                    state: stage.approach

                    onEntered: (mouse)=>{
                        if (stage.approach === Stage.Wound) {
                            manipulatePane.cursorVisible = true;
                        }
                    }

                    onExited: (mouse)=>{
                        if (stage.approach === Stage.Wound) {
                            manipulatePane.cursorVisible = false;
                        }
                    }

                    onPressed:  (mouse)=>{
                        if ((mouse.button  === Qt.MiddleButton)) {
                            if (stage.approach === Stage.Wound) {
                                manipulatePane.visible = false;
                            }
                        } else if ((mouse.button === Qt.LeftButton) )   {
                            if (stage.approach === Stage.Select) {
                                stage.cleanSelectArea();
                            } else if(stage.approach === Stage.Wound) {
                                manipulatePane.cursorShape = stage.woundMode === Halve.WM_ABRADE? "qrc:/assets/images/abrade.svg": "qrc:/assets/images/pierce.svg";
                                stage.wound(mouse.x, mouse.y, abradeRadius, Stage.Pressed)
                            }
                        }
                    }

                    onReleased: (mouse)=>{
                        if ((mouse.button === Qt.MiddleButton)) {
                            if (stage.approach === Stage.Wound) {
                                manipulatePane.visible = true;
                            }
                        } else if ((mouse.button === Qt.LeftButton)) {
                            if (stage.approach === Stage.Wound)   {
                                stage.wound(mouse.x, mouse.y,abradeRadius, Stage.Released)
                                manipulatePane.cursorShape = null;
                                lastAbradePos = null;
                            } else if (stage.approach === Stage.Select)   {
                                stage.applySelectArea(manipulatePane.polygonPoints);
                            }
                        }
                    }

                    property var lastAbradePos:null
                    property int abradeRadius:30

                    onPositionChanged: (mouse)=> {
                       if (stage.approach === Stage.Wound) {
                            if (lastAbradePos !== null ) {
                                var distance = Utils.calculateDistance(lastAbradePos.x, lastAbradePos.y, mouse.x, mouse.y);
                                if (distance < 20) {
                                  return;
                                }
                            }
                            stage.wound(mouse.x, mouse.y, abradeRadius, Stage.Moving);
                            lastAbradePos = {x: mouse.x, y: mouse.y}
                       }
                    }
                }

                ToolButton {
                    id:extcButton
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 3
                    anchors.topMargin: 10
                    focusPolicy: Qt.NoFocus
                    icon {
                        source: "qrc:/assets/images/menu.svg"
                        height: 17
                        width:17
                    }
                    radius:width/2
                    visibleBackground:true
                    onClicked: moreMenuPopup.open()
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Options")

                    Menu {
                        id: moreMenuPopup
                        y: parent.height
                        x: parent.width - width

                        contentItem: Pane {
                            focusPolicy: Qt.ClickFocus
                            StageOptionPane {
                                anchors.fill: parent
                                options: root.options
                            }
                        }
                    }
                }


                ScalarBar {
                    id:scalarBar
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    height: stage.height - extcButton.height - 50
                    anchors.bottomMargin: 5
                    anchors.rightMargin: 5
                    width: 60
                    visible: root.options.scalarBar
                    profile:root.profile
                }


                Item {
                    visible: root.options.showOrientation
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    width:100
                    height: 100
                    Orientation {
                        id:orientation
                        anchors.fill: parent
                        opacity: 0.9
                        profile:root.profile
                        cameraInfo:stage.cameraInfo
                        Connections {
                            target: root.profile
                            function onOrientaionMatrixChanged(){
                                orientation.onOrientaionMatrixChanged();
                            }
                        }
                    }

                    Label {
                        visible: stage.azimuthName !== ""
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        font.family: "Helvetica"
                        font.pointSize: 15
                        font.bold: true
                        font.weight: 600
                        color: "Yellow"
                        text:stage.azimuthName
                    }
                }


                Item {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.topMargin: 20
                    anchors.leftMargin: 10

                    ColumnLayout {
                        anchors.fill: parent
                        ToolButton {
                            Layout.alignment: Qt.AlignHCenter
                            icon {
                                source: "qrc:/assets/images/3d_point.svg"
                                height: 17
                                width:17
                            }
                            radius:width/2
                            visibleBackground:true
                            onClicked:stage.options.show3dPoints = checked
                            checked: stage.options.show3dPoints
                            checkable: true
                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("3D point")

                        }
                        ToolButton {
                            Layout.alignment: Qt.AlignHCenter
                            icon {
                                source: "qrc:/assets/images/surface_point.svg"
                                height: 17
                                width:17
                            }
                            radius:width/2
                            visibleBackground:true
                            onClicked:stage.options.showSurfacePoints = checked
                            checked: stage.options.showSurfacePoints
                            checkable: true
                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Surface point")
                        }
                        ToolButton {
                            Layout.alignment: Qt.AlignHCenter
                            icon {
                                source: "qrc:/assets/images/dissolve_point.svg"
                                height: 17
                                width:17
                            }
                            radius:width/2
                            visibleBackground:true
                            onClicked:stage.options.showDissolvePoints = checked
                            checked: stage.options.showDissolvePoints
                            checkable: true
                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Dissolve point")
                        }
                    }
                }

                Item {
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: Math.min(300, parent.width)
                    anchors.bottomMargin: 10
                    height: 30
                    RowLayout {
                        anchors.fill: parent
                        Label {
                            text:qsTr("Opacity")
                            font.bold: true
                            font.pixelSize: 13
                        }

                        Slider {
                            id:transparencySlider
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            from: 0.00
                            value: root.options.transparency
                            to: 1.00
                            stepSize:0.01
                            onMoved: {
                                root.options.transparency = value
                            }
                        }
                    }
                }

                Item {
                    visible: root.combined.mode === Halve.CHANNELMODE_ELECTRICAL
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.topMargin: 5
                    height: 40
                    width: 80
                    Rectangle {
                        anchors.fill: parent
                        color: "#CD3333"
                        opacity: 0.9
                        radius: 10
                        border.color: "white"
                        Label {
                            text: root.combined.bloodPoolImpedance.toFixed(0)
                            color: "white"
                            font.weight: 1200
                            font.pixelSize: 18
                            anchors.centerIn: parent
                        }
                    }

                }

            }

            onInitRender:stage.resetRender()
            onClicked:(x,y)=>{
                if (stage.approach === Stage.Dissolve) {
                    stage.applyDissolve(x,y)
                } else {
                    stage.applyPick(x,y)
                }
            }
            onPointPicked:(x,y,z)=>applyPick(x,y, z)

            Timer {
                interval: 500;
                running: stage.hightlight
                repeat: true
                onTriggered: stage.refreshHighlight()
            }

        }
    }

    Component.onCompleted: function() {
        orientation.setNewAP();
    }
}
