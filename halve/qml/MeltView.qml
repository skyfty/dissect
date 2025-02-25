import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import QtCharts 2.5
import QtQuick.Dialogs
import "utils.js" as Utils
import "models"

Rectangle {
    id:root
    required property var profile
    property var options: root.profile.meltOptions
    required property var breathSurvey
    property var meltSamplingElectrode : options.meltSamplingElectrode
    property var consultListIndex: options.consultListIndex
    property var consultListCount: options.consultListCount
    property var setting: root.profile.meltSetting
    property var baseIndex: setting.baseIndex
    property var currentReseau : root.profile.currentReseau
    required property var catheterListModel;
    required property var combined
    required property var channel
    required property var mappingPointGroupListModel
    property var scalarModel
    property bool tadkState: root.combined.state === ChannelReplica.State_Tracking &&  setting.consultElectrode !== ""  && root.currentReseau.apparent && root.currentReseau.pointNumber > 0
     // property bool tadkState:true
    readonly property real dpi: Screen.pixelDensity * 25.4
    readonly property int meltScaleplateStep: 10

    color:"#191919"
    clip: true


    HotkeyAction {
        enabled: root.currentReseau.apparent && root.currentReseau.pointNumber > 0
        profile:root.profile
        name: "tadkMapping"
        onActivated: {
            if (root.profile.meltState === Halve.STAND) {
                root.profile.meltState = Halve.TADK;
            }
        }
    }
    HotkeyAction {
        enabled:tadkState
        profile:root.profile
        name: "standMapping"
        onActivated: {
            if (root.profile.meltState === Halve.TADK) {
                root.profile.meltState = Halve.STAND;
            } else {
                if (root.profile.meltState !== Halve.TADK) {
                    if (root.profile.meltState === Halve.REVIEW) {
                        dataSource.adjust();
                    } else {
                        dataSource.strik();
                    }
                }
                root.profile.meltState = Halve.TADK;
            }
        }
    }

    HotkeyAction {
        profile:root.profile
        name: "previousConsult"
        onActivated: previousAction.trigger()

    }

    HotkeyAction {
        profile:root.profile
        name: "nextConsult"
        onActivated: nextAction.trigger()
    }
    ChannelDataFilter {
        id:currentChannelDataFilter
        profile:root.profile
        samplingRate:AppSettings.samplingRate
    }

    MeltDataSource{
        id:dataSource
        profile:root.profile
        combined:root.combined
        channel:root.channel
        channelDataFilter:currentChannelDataFilter
        meltSetting:root.profile.meltSetting
        mappingSetting:root.profile.mappingSetting
        meltOptions:root.profile.meltOptions
        consultElectrodeId : root.setting.consultElectrodeId
        breathSurvey:root.breathSurvey
        Component.onCompleted: {
            root.profile.meltState = Halve.TADK
        }
        onStriked: {
            root.setting.striked = true;
        }
        onMeltOptionsChanged: reinitChartList()

        onConsultElectrodeIdChanged: {
            if (meltOptions.id !== 0) {
                dataSource.reloadSeriesData();
            } else {
                if (dataSource.consultElectrodeId !== ""){
                    root.profile.pulseElectrodeId = dataSource.consultElectrodeId
                } else {
                    root.profile.pulseElectrodeId = AppSettings.defaultPulseElectrodeId
                }      
                dataSource.update();
            }
        }
    }

    function updateDataSource() {
        if (root.options.id !== 0) {
            dataSource.reloadSeriesData();
        } else {
            dataSource.update();
        }
    }


    Timer {
        id:calculateMaxTimer
         interval: 100; running: false; repeat: false
         onTriggered: {
             calculateMax()
             scaleplate.reset()
         }
    }

    function calculateMax() {
        var scaleplateWidth = scaleplate.width / dpi * 25.4;
        root.setting.max = scaleplateWidth /  root.setting.speed * AppSettings.samplingRate;;
    }

    function calculateMeltIndex() {
        root.setting.baseIndex = root.setting.max/2;
        root.setting.leftIndex = root.setting.baseIndex + root.setting.leftShifting;
        root.setting.rightIndex = root.setting.baseIndex + root.setting.rightShifting;
    }


    Timer {
        id:meltTimer
        interval: root.setting.refurbishSpeed;
        running: root.setting.consultElectrodeId !== "" && root.profile.meltState === Halve.TADK
        repeat: true
        onTriggered: dataSource.update()
    }

    CasketCatheterListModel{
        id:catheterListModel
        profile:root.profile
    }
    ElectrodeNodeSelectListModel{
        id:currentElectrodeNodeListModel
        profile:root.profile
    }

    onBaseIndexChanged:{
        calculateMaxTimer.restart();
    }

    onWidthChanged: {
        calculateMaxTimer.restart();
        dataSource.resetSeriesSize();
    }

    MeltConsultElectrodeListModel{
        id:meltConsultElectrodeListModel
        profile: root.profile
        consultElectrodeId: root.setting.consultElectrodeId
        onDeleted: {
            dataSource.setConsultSeries(null);
            root.setting.consultElectrodeId = null;
            meltConsultElectrodeListModel.removeRows(0, 1);
        }
    }
    Component {
        id:chartListLoader
        ColumnLayout {
            id:chartListRoot
            spacing: 0
            Connections {
                target: root.profile.meltOptions.appointElectrode
                function onSizeChanged(){
                    updateDataSource();
                }
            }
            ListView {
                Layout.fillWidth: true
                Layout.maximumHeight: 100
                Layout.preferredHeight: 100
                model: meltConsultElectrodeListModel
                flickableDirection: Flickable.AutoFlickIfNeeded
                snapMode:ListView.NoSnap
                delegate: MeltConsultChart {
                    id:meltConsultChart
                    anchors.fill: parent
                    setting:root.profile.meltSetting
                    Component.onCompleted: {
                        dataSource.setConsultSeries(meltConsultChart.chart.series(0));
                    }
                }
            }
            MeltChartList{
                id:meltChartList
                Layout.fillWidth: true
                Layout.fillHeight:true
                source:dataSource
                setting:root.profile.meltSetting
                profile:root.profile
                currentRowId:root.profile.currentMappingPointId
                model: MeltChannelChartListModel {
                    profile:root.profile
                }
            }

            Timer {
                 interval: 300;
                 running: true;
                 repeat: false
                 triggeredOnStart: false
                 onTriggered: {
                    calculateMeltIndex();
                    calculateMaxTimer.restart();
                    scalarModel.reset();
                    slitherWindow.reset();
                    dataSource.resetSeriesSize();
                    dataSource.reloadSeriesData();
                 }
             }
        }
    }

    function reinitChartList() {
        dataSource.setConsultSeries(null);
        chartListLoaderStackView.clear();
        chartListLoaderStackView.push(chartListLoader);
    }


    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        RowLayout {
            id:toolbar
            Layout.fillWidth: true
            Layout.maximumHeight:  34
            Layout.rightMargin: 3
            Layout.leftMargin: 3

            ComboBox {
                textRole: "text"
                valueRole: "value"
                Layout.alignment: Qt.AlignVCenter
                Layout.preferredWidth: 100
                model: SpeedModel{}

                onActivated: {
                    root.setting.speed = currentValue;                    
                    calculateMaxTimer.restart();
                    root.reinitChartList();
                }

                Component.onCompleted:{
                    currentIndex = indexOfValue(root.setting.speed)
                }
            }
            ToolSeparator {}
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/gather.png"
                    height: 19
                    width:19
                }

                action: Action {
                    enabled: !root.setting.striked
                    onTriggered: consultSettingPopup.open()
                }
                Menu {
                    id: consultSettingPopup
                    y: parent.height
                    contentItem: Pane {
                        focusPolicy: Qt.ClickFocus
                        ConsultSettingPane  {
                            anchors.fill: parent
                            setting:root.profile.meltSetting
                            electrodeNodeListModel:currentElectrodeNodeListModel
                        }
                    }
                    onAboutToHide: {
                        root.profile.meltOptionsDb.save()
                        root.profile.mappingPointGroupDb.save()
                    }
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Consult Settings")
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/sampling.svg"
                    height: 19
                    width:19
                }
                enabled: root.setting.consultElectrodeId !== ""
                onClicked: samplingSettingPopup.open()
                Menu {
                    id: samplingSettingPopup
                    y: parent.height
                    contentItem: Pane {
                        focusPolicy: Qt.ClickFocus
                        ColumnLayout {
                            anchors.fill: parent
                            spacing:7
                            MeltSignSettingPane {
                                Layout.fillWidth: true
                                profile: root.profile
                                bodyVisible:false
                                catheterListModel:root.catheterListModel
                                electrodeList:meltSamplingElectrode
                                electrodeListFilter:root.profile.meltOptions.appointElectrode
                                consultElectrodeId:root.profile.meltSetting.consultElectrodeId
                            }
                            ColumnLayout {
                                Label{
                                    text: qsTr("Perception")
                                    Layout.fillWidth: true
                                }
                                ComboBox {
                                    Component.onCompleted: currentIndex = indexOfValue(meltSamplingElectrode.samplingPerception)
                                    onActivated:{
                                        meltSamplingElectrode.samplingPerception = currentValue;
                                    }
                                    textRole: "text"
                                    valueRole: "value"
                                    Layout.fillWidth: true
                                    model: PerceptionModel{}
                                }
                            }
                        }
                    }
                    onAboutToHide: {
                        root.profile.meltOptionsDb.save()
                        root.profile.mappingPointGroupDb.save()
                    }
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Sampling settings")
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                icon {
                    source: "qrc:/assets/images/sign.png"
                    height: 19
                    width:19
                }
                onClicked: signSettingPopup.open();
                Menu {
                    id: signSettingPopup
                    y: parent.height
                    contentItem: Pane {
                        focusPolicy: Qt.ClickFocus
                        RowLayout {
                            anchors.fill: parent
                            MeltSignSettingPane {
                                profile: root.profile
                                catheterListModel:root.catheterListModel
                                electrodeList:root.profile.meltOptions.appointElectrode
                                electrodeListFilter:meltSamplingElectrode
                                consultElectrodeId:root.profile.meltSetting.consultElectrodeId
                            }
                        }
                    }
                    onAboutToHide: {
                        root.profile.meltOptionsDb.save()
                    }
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Signal Settings")
            }
            ToolSeparator {}

            ToolButton {
                Layout.alignment: Qt.AlignHCenter
                icon {
                    source: "qrc:/assets/images/ruler_lat.png"
                    height: 19
                    width:19
                }

                checked: setting.rulerType === 1
                onClicked: {
                   setting.rulerType = (setting.rulerType === 1?0:1)
                }

                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Ruler of LAT")
            }

            ToolButton {
                Layout.alignment: Qt.AlignHCenter
                icon {
                    source: "qrc:/assets/images/ruler_voltage.png"
                    height: 19
                    width:19
                }
                checked: setting.rulerType === 2
                onClicked: {
                    setting.rulerType = (setting.rulerType === 2?0:2)
                }

                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Ruler of voltage")
            }
            ToolButton {
                Layout.alignment: Qt.AlignHCenter
                icon {
                    source: "qrc:/assets/images/regulate.svg"
                    height: 19
                    width:19
                }

                onClicked: regulateSettingPopup.open();
                Menu {
                    id: regulateSettingPopup
                    y: parent.height
                    contentItem: Pane {
                        focusPolicy: Qt.ClickFocus
                        RegulateSettingPane {
                            anchors.fill: parent
                            setting:root.profile.meltSetting
                        }
                    }
                    onAboutToHide: {
                        root.profile.mappingPointGroupDb.save()
                    }
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Regulate")
            }
      

        }
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout {
                Layout.fillHeight: true
                Layout.maximumWidth: 27

                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        source: root.profile.meltState != Halve.TADK?"qrc:/assets/images/play.png":"qrc:/assets/images/pause.png"
                        height: 19
                        width:19
                    }

                    enabled:tadkState
                    onClicked: {
                        if (root.profile.meltState === Halve.TADK) {
                            root.profile.meltState = Halve.STAND;
                        } else {
                            root.profile.meltState = Halve.TADK;
                        }
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: root.profile.meltState !== Halve.TADK?qsTr("Play"):qsTr("Pause")
                }
                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/tadk.svg"
                        height: 19
                        width:19
                    }

                    enabled: root.profile.meltState === Halve.REVIEW || root.profile.meltState === Halve.STAND
                    onClicked: {
                        if (root.profile.meltState === Halve.REVIEW) {
                            dataSource.adjust();
                        } else {
                            dataSource.strik();
                        }
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Start the calibration")
                }
                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    action: Action {
                        id: previousAction
                        enabled:root.profile.meltState !==  Halve.TADK && root.currentReseau.apparent && root.currentReseau.pointNumber > 0
                        onTriggered: {
                            dataSource.showConsultData(MeltDataSource.PREVIOUS);
                        }
                    }

                    icon {
                        source: "qrc:/assets/images/previous.svg"
                        height: 19
                        width:19
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Previous Consult")
                }
                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/next.svg"
                        height: 19
                        width:19
                    }

                    action: Action {
                        id: nextAction
                        enabled:root.profile.meltState !==  Halve.TADK && root.currentReseau.apparent && root.currentReseau.pointNumber > 0
                        onTriggered: {
                            dataSource.showConsultData(MeltDataSource.NEXT);
                        }
                    }

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Next Consult")
                }

                ToolSeparator{orientation:Qt.Horizontal;Layout.fillWidth: true}
                ToolButton {
                    Layout.alignment: Qt.AlignVCenter
                    icon {
                        source: "qrc:/assets/images/reset.svg"
                        height: 17
                        width:17
                    }
                    action: Action {
                        id: resetAction
                        onTriggered: {
                            resetConfirmDialog.open();
                        }
                    }

                    ConfirmDialog {
                        id: resetConfirmDialog
                        text: qsTr("After the configuration modification\r\na new map group needs to be created for mapping?")
                        onAccepted: {
                            var newGroupIndex = mappingPointGroupListModel.clone(root.profile.currentMappingPointGroup);
                            var newGroupId = mappingPointGroupListModel.data(mappingPointGroupListModel.index(newGroupIndex, 0), MappingPointGroupListModel.Id);
                            root.profile.currentMappingPointGroup = newGroupId
                        }
                    }

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Reset")
                }
                      
                ToolButton {
                    Layout.alignment: Qt.AlignHCenter
                    icon {
                        source: "qrc:/assets/images/strik.svg"
                        height: 17
                        width:17
                    }
                    onClicked: {
                        dataSource.restrikConsultData();
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Recalculate Sampling")
                }
                Item {
                    Layout.fillHeight: true

                }
            }

            Rectangle {
                id:channelChartView
                Layout.fillWidth: true
                Layout.fillHeight: true
                color:"black"
                ColumnLayout {
                    id:channelChartViewRow
                    spacing: 0
                    anchors.fill: parent
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 0
                        Layout.maximumHeight: 20
                        Label {
                            text:(root.consultListIndex === -1?"?":(root.consultListIndex + 1)) + "/" + root.consultListCount
                            verticalAlignment : Text.AlignVCenter
                            horizontalAlignment:Text.AlignRight
                            Layout.preferredWidth: AppSettings.channelChartListLabelWidth
                            rightPadding: 10
                            font.pixelSize: 13
                            font.bold: true
                            Layout.fillHeight: true
                        }
                        Scaleplate {
                            id:scaleplate
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            step:100

                            function reset() {
                                var scaleplateWidth = scaleplate.width / dpi * 25.4;
                                var step = root.setting.max / (scaleplateWidth / root.meltScaleplateStep)
                                if (step <= 0) {
                                    return;
                                }
                                if (step >= 100) {
                                    scaleplate.step =  Math.floor(step / 100) * 100
                                } else {
                                    scaleplate.step =  Math.floor(step / 10) * 10
                                }
                                var maxValue = root.setting.max - root.setting.baseIndex;
                                scaleplate.maxValue =  Math.floor(maxValue / AppSettings.samplingRate * 1000)
                                scaleplate.minValue =  -Math.floor((root.setting.max - maxValue) / AppSettings.samplingRate * 1000);
                            }
                        }
                    }
                    StackView {
                        id:chartListLoaderStackView
                        Layout.fillWidth: true
                        Layout.fillHeight:true
                        initialItem: Item{}
                        replaceEnter:Transition {
                            PropertyAnimation {
                                property: "opacity"
                                from: 0
                                to:1
                                duration: 300
                            }
                        }

                        replaceExit: Transition {
                            PropertyAnimation {
                                property: "opacity"
                                from: 1
                                to:0
                                duration: 300
                            }
                        }
                    }
                }

                SlitherWindow {
                    id: slitherWindow
                    anchors.fill: parent
                    anchors.leftMargin: AppSettings.channelChartListLabelWidth
                    setting:root.profile.meltSetting
                    onAccepted: {
                        root.updateDataSource();
                        scalarModel.reset();
                        root.profile.mappingPointGroupDb.save()
                        root.profile.meltSetting.shiftChanged = true
                    }
                    visible: setting.rulerType === 0
                }
            }
        }
    }
}
