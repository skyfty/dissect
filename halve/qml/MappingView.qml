import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtQuick.Dialogs
import "utils.js" as Utils
import "models"

Pane {
    id:root
    required property var profile
    property var mapping: root.profile.mapping
    required property var mappingPointGroupListModel
    property var scalarModel
    padding: 1

    ChannelDataFilter {
        id:currentChannelDataFilter
        samplingRate:AppSettings.samplingRate
        profile:root.profile
    }
    MappingPointsListModel {
        id:mappingPointsListModel
        profile:root.profile
   }
    MappingPointSortFilterProxyModel {
        id:mappingPointsModel
        profile:root.profile
        sortRole:mapping.sortRole
        sourceModel: mappingPointsListModel
        onMappingChanged: {
            mappingPointsModel.invalidate();
        }
    }


    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            Layout.fillWidth: true
            Layout.maximumHeight:  34
            Layout.alignment:Qt.AlignVCenter
            spacing:3
            Layout.margins: 1
            MappingComboBox {
                id:mappingComboBox
                Layout.preferredWidth: 130
                model:mappingPointGroupListModel
                Component.onCompleted: {
                    currentIndex = indexOfValue(root.profile.currentMappingPointGroup);
                }
                onActivated:(currentValue)=>{
                    root.profile.currentMappingPointGroup = currentValue
                }
                onAdd:{
                    currentIndex = mappingPointGroupListModel.clone(root.profile.currentMappingPointGroup);
                    root.profile.currentMappingPointGroup = currentValue
                }

                onEdit:(index, text)=> {
                    var idx = mappingPointGroupListModel.indexOfName(text);
                    if (idx === -1) {
                        mappingPointGroupListModel.setData(mappingPointGroupListModel.index(index, 0), text, MappingPointGroupListModel.Name)
                    }
                }

                onRemove: {
                    mappingPointGroupListModel.removeRows(index, 1)
                }
            }

            ComboBox {
                id:mappingTypeComboBox
                focusPolicy:Qt.NoFocus
                textRole: "text"
                valueRole: "value"
                Layout.preferredWidth: 70
                model: MeltModel{}
                Component.onCompleted: currentIndex = indexOfValue(root.profile.mappingType)
                onActivated: {
                    root.profile.mappingType = currentValue;
                }
            }

            Item {
                Layout.fillWidth: true
            }

            ComboBox {
                id:sortComboBox
                textRole: "text"
                valueRole: "value"
                focusPolicy:Qt.NoFocus
                model: ListModel {
                    ListElement{
                        value: MappingPointSortFilterProxyModel.COLLECTING_TIME_ASC
                        text: qsTr("Collecting time (ASC)")
                    }
                    ListElement{
                        value: MappingPointSortFilterProxyModel.COLLECTING_TIME_DESC
                        text: qsTr("Collecting time (DESC)")
                    }
                    ListElement{
                        value: MappingPointSortFilterProxyModel.LAT_ASC
                        text: qsTr("LAT (ASC)")
                    }
                    ListElement{
                        value: MappingPointSortFilterProxyModel.LAT_DESC
                        text: qsTr("LAT (DESC)")
                    }
                    ListElement{
                        value: MappingPointSortFilterProxyModel.VOLTAGE_ASC
                        text: qsTr("Voltage (ASC)")
                    }
                    ListElement{
                        value: MappingPointSortFilterProxyModel.VOLTAGE_DESC
                        text: qsTr("Voltage (DESC)")
                    }
                }
                Component.onCompleted: {
                    currentIndex = indexOfValue(mapping.sortRole)
                    mappingPointsModel.sort(0, Qt.AscendingOrder);
                }
                onActivated: {
                    mapping.sortRole = sortComboBox.currentValue
                    mappingPointsTable.clearCurrentIndex();
                    mappingPointsModel.sort(0, Qt.AscendingOrder);

                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                Layout.fillHeight: true
                Layout.maximumWidth: 27
                ToolButton {
                    icon {
                        source: "qrc:/assets/images/mapping.png"
                        height: 19
                        width:19
                    }

                    Component {
                        id:mappingSettingPopupComponent
                        Menu {
                            id: mappingSettingPopup
                            y: parent.height
                            contentItem:Pane {
                                focusPolicy: Qt.ClickFocus
                                MappingSettingPane {
                                    anchors.fill: parent
                                    profile:root.profile
                                }
                            }
                            onAboutToHide: {
                                root.profile.save()
                                mappingSettingPopup.destroy();
                            }
                        }
                    }

                    onClicked: {
                        mappingSettingPopupComponent.createObject(this).open();
                    }

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("The calibration setting")
                }

                ToolSeparator{orientation:Qt.Horizontal;Layout.fillWidth: true}
                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/checkbox_group.svg"
                        height: 19
                        width:19
                    }
                    onClicked:  {
                        if (childButtonGroup.checkState == Qt.Unchecked) {
                            childButtonGroup.checkState = Qt.Checked;
                        } else {
                            childButtonGroup.checkState = Qt.Unchecked;
                        }
                    }
                    enabled:mappingPointsTable.count > 0
                }

                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/erase.png"
                        height: 19
                        width:19
                    }
                    onClicked:  {
                        deleteConfirmDialog.open();
                    }
                    enabled: childButtonGroup.checkState != Qt.Unchecked
                    ConfirmDialog {
                        id: deleteConfirmDialog
                        text: qsTr("Are you sure you want to delete this?")
                        onAccepted: {
                            mappingPointsModel.removeRows(childButtonGroup.getCheckedRowIds());
                        }
                    }

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Delete")
                }
                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/delete_group.svg"
                        height: 19
                        width:19
                    }
                    onClicked:  {
                        deleteCusterConfirmDialog.open();
                    }
                    enabled:  childButtonGroup.checkState != Qt.Unchecked
                    ConfirmDialog {
                        id: deleteCusterConfirmDialog
                        text: qsTr("Are you sure you want to delete the other records in the group along with this record?")
                        onAccepted: {
                            mappingPointsModel.removeCuster(childButtonGroup.getCheckedRowIds());
                        }
                    }

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Delete Group")
                }

                MappingPointGroupConsultFilterListModel {
                    id:mappingPointGroupConsultFilterListModel
                    sourceModel: root.mappingPointGroupListModel
                    currentMappingPointGroup:root.profile.currentMappingPointGroup
                    onCurrentMappingPointGroupChanged: {
                        mappingPointGroupConsultFilterListModel.invalidate();
                    }
                }

                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/attorn.svg"
                        height: 19
                        width:19
                    }

                    Component {
                        id:attornMappingPointsSelectDialogComponent
                        MappingPointGroupSelectDialog {
                            id: attornMappingPointsSelectDialog
                            model:mappingPointGroupConsultFilterListModel
                            text: qsTr("Are you sure you want to attorn the records to other group?")
                            onAccepted: {
                                if (root.profile.currentMappingPointGroup !== currentValue) {
                                    mappingPointCopyer.attorn(currentValue,childButtonGroup.getCheckedRowIds())
                                }
                            }
                            onAdd: {
                                attornMappingPointsSelectDialog.currentRow = mappingPointGroupConsultFilterListModel.clone(root.profile.currentMappingPointGroup);
                            }
                        }
                    }

                    onClicked:  {
                        attornMappingPointsSelectDialogComponent.createObject(this).open();
                    }
                    enabled:  childButtonGroup.checkState != Qt.Unchecked &&  mappingPointRestrik.progressValue === 0

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Attorn")
                }
                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/copy.svg"
                        height: 19
                        width:19
                    }
                    onClicked:  {
                        copyConfirmDialogComponent.createObject(this).open();
                    }
                    enabled:  childButtonGroup.checkState != Qt.Unchecked &&  mappingPointRestrik.progressValue === 0

                    Component {
                        id:copyConfirmDialogComponent
                        MappingPointGroupSelectDialog {
                            id: copyConfirmDialog
                            model:mappingPointGroupConsultFilterListModel
                            text: qsTr("Are you sure you want to copy the records to other group?")
                            onAccepted: {
                                if (root.profile.currentMappingPointGroup !== copyConfirmDialog.currentValue) {
                                    mappingPointCopyer.copy(copyConfirmDialog.currentValue,childButtonGroup.getCheckedRowIds())
                                }
                            }
                            onAdd: {
                                copyConfirmDialog.currentRow = mappingPointGroupConsultFilterListModel.clone(root.profile.currentMappingPointGroup);
                            }
                        }
                    }

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Copy")
                }

                MappingPointCopyer {
                    id:mappingPointCopyer
                    profile:root.profile
                    channelDataFilter:currentChannelDataFilter
                    onAttornFinished: (ids)=>{
                        mappingPointsListModel.removeRows(ids);
                    }
                    onProgressValueChanged: {
                        progressBarPanel.progressValue = progressValue;
                    }
                }

                ToolButton {
                    id:btnStrik
                    Layout.alignment: Qt.AlignHCenter
                    icon {
                        source: "qrc:/assets/images/strik.svg"
                        height: 17
                        width:17
                    }
                    onClicked: {
                        strikConfirmDialog.open();
                    }
                    enabled: mappingPointRestrik.progressValue === 0

                    background: Rectangle {
                        id: panel
                        implicitWidth: 20
                        implicitHeight: 20
                        visible: btnStrik.down || btnStrik.visualFocus || (btnStrik.enabled && btnStrik.hovered) || root.profile.meltSetting.shiftChanged
                        color: btnStrik.down? "#46607c": btnStrik.enabled && btnStrik.hovered?"#464646":"#3c3c3c"
                        radius: 2
                        Rectangle {
                            id: animatedRectangle
                            x: 1; y: 1
                            width: parent.width - 2
                            height: parent.height - 2
                            border.color: Fusion.innerContrastLine
                            color: "transparent"
                            visible: root.profile.meltSetting.shiftChanged
                            radius: 2
                            // 颜色动画
                            SequentialAnimation {
                                running: root.profile.meltSetting.shiftChanged
                                loops: Animation.Infinite // 无限循环动画
                                ColorAnimation {
                                    target: animatedRectangle // 目标为此矩形
                                    property: "color"
                                    from: "transparent"
                                    to: "red"
                                    duration: 2000 // 从红到白的动画时长（毫秒）
                                }
                                ColorAnimation {
                                    target: animatedRectangle // 目标为此矩形
                                    property: "color"
                                    from: "red"
                                    to: "transparent"
                                    duration: 2000 // 从白到红的动画时长（毫秒）
                                }
                            }
                        }
                    }

                    MappingPointRestrik {
                        id:mappingPointRestrik
                        profile:root.profile
                        channelDataFilter:currentChannelDataFilter
                        onFinished: {
                            root.profile.meltSetting.shiftChanged = false
                            mappingPointsListModel.reload();
                        }

                        onProgressValueChanged: {
                            progressBarPanel.progressValue = progressValue;

                        }
                    }

                    ConfirmDialog {
                        id: strikConfirmDialog
                        text: qsTr("Are you sure you want to recalculate the measurement results?")
                        onAccepted: {
                            mappingPointRestrik.restrik(root.profile.currentMappingPointGroup,childButtonGroup.getCheckedRowIds())
                        }
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Recalculate Sampling")
                }
                Item {
                    Layout.fillHeight: true

                }

            }

            ButtonGroup {
                 id: childButtonGroup
                 exclusive: false
                 function getCheckedRowIds() {
                     var rowIds = [];
                     for(var i = 0; i < childButtonGroup.buttons.length; ++i) {
                         if (childButtonGroup.buttons[i].checked) {
                             rowIds.push(childButtonGroup.buttons[i].rowId);
                         }
                     }
                     return rowIds;
                 }
            }
            MappingTable {
                id:mappingPointsTable
                Layout.fillWidth: true
                Layout.fillHeight: true
                model:mappingPointsModel
                mappingType: mappingTypeComboBox.currentValue
                buttonGroup:childButtonGroup
                onCurrentRowIdChanged: {
                    if (currentRowId != -1) {
                        root.profile.meltState = Halve.REVIEW;
                        root.profile.currentMappingPointId = currentRowId;
                    } else {
                        root.profile.currentMappingPointId = -1;
                    }
                }
                Connections {
                    target:root.profile
                    function onCurrentMappingPointIdChanged(id){
                        if (mappingPointsModel.currentRowId === root.profile.currentMappingPointId) {
                            return;
                        }
                        if (root.profile.currentMappingPointId === -1) {
                            mappingPointsTable.scrollTo(-1);
                        } else {
                            mappingPointsTable.scrollTo(root.profile.currentMappingPointId);
                        }
                    }

                    function onCurrentMappingPointGroupChanged(){
                        mappingPointsTable.clearCurrentIndex();
                        mappingComboBox.currentIndex = mappingComboBox.indexOfValue(root.profile.currentMappingPointGroup);
                    }

                    function onMeltStateChanged() {
                        if (root.profile.meltState !== Halve.REVIEW) {
                            mappingPointsTable.clearCurrentIndex()
                        }
                    }
                }

                Item {
                    id:progressBarPanel
                    property real progressValue: 0.0
                    visible: progressValue > 0 && progressValue < 1.0
                    anchors.fill: parent
                    Rectangle {
                        anchors.fill: parent
                        color: "black"
                        opacity: 0.6
                    }

                    Item {
                        anchors.centerIn: parent
                        width: parent.width - 30
                        anchors.margins: 10
                        height: 20
                        ProgressBar {
                            id:progressBar
                            anchors.fill: parent
                            anchors.margins: 1
                            value: progressBarPanel.progressValue
                        }
                        Label {
                            anchors.centerIn: parent
                            text:Math.round(progressBarPanel.progressValue * 100) + "%"
                            font.bold: true
                            font.pixelSize: 14
                        }
                    }
                }


                BusyIndicator {
                    running: !root.profile.pointValidCheck
                    anchors.centerIn: parent
                }

            }
        }
    }



}
