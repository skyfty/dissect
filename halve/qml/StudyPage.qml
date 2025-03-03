import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

import Halve 1.0
import com.kdab.dockwidgets 2.0 as KDDW

import "mediaplayer"
import "models"

Study {
    id: root
    obscurity:currentObscurity
    combined:currentCombined
    property var reproduceOptions: root.profile.reproduceOptions
    
    Combined {
        id:currentCombined
        profile:root.profile
        channel:root.channel
        displacement:AppSettings.electricDisplacement
        magnetismTrainRate:AppSettings.magnetismTrainRate
        coefficient:AppSettings.coefficient
        electricCenterShifting:AppSettings.electricCenter
        keepSave: AppSettings.keepSave
        onTrainingChanged: {
            root.profile.catheterDb.save();
            notebookDataSource.autoRecord(currentCombined.training?"Start training":"End training");
        }
    }

    CatheterListModel{
        id:currentCatheterListModel
        profile:root.profile
    }

    CatheterTableModel {
        id:catheterTableModel
        profile:root.profile
        ecgColor:InterfaceSettings.ecgElectrodeColor
        channeMode:root.profile.channelMode
        onRowsInserted: {
            dockCatheterView.setAsCurrentTab();
        }
        catheterTypeDb:ProfileManager.catheterRepertory.catheterTypeDb
    }

    MappingPointGroupListModel{
        id:currentMappingPointGroupListModel
        profile:root.profile;
    }
    ReseauListModel {
        id:reseauListModel
        profile:root.profile
    }

    AzimuthModel {
        id:azimuthModel
        profile: root.profile
    }
    Obscurity {
        id:currentObscurity
        profile: root.profile
        combined:root.combined
        breathSurvey:currentBreathSurvey
        Component.onCompleted: {
            currentObscurity.init()
        }
    }

    BreathSurvey {
        id:currentBreathSurvey
        profile:root.profile
        combined:root.combined
        samplingRate:AppSettings.breathSamplingRate
        breathGatingRatio:AppSettings.breathGatingRatio
        samplingFilter:AppSettings.breathSamplingFilter
        breathSamplingValidLimit:AppSettings.breathSamplingValidLimit
        onPantSampleFinished: {
            dockBreatheView.show();
            root.profile.breathOptions.breatheGate = false
            root.profile.breathOptions.enabledCompensate = true
            root.profile.breathOptions.breatheCompensation = true
            root.profile.breathOptions.breatheRestrictRenovate = true
        }
    }

    AlbumModel {
        id:albumModel
        profile:root.profile
        Component.onDestruction: {
            albumModel.stopScreenGrap();
            albumModel.stopScreenRecord();
        }
    }

    StageScalar {
        id:currentStageScalar
        profile:root.profile
        basis:AppSettings.basis
    }

    NotebookDataSource{
        id:notebookDataSource
        profile:root.profile
        channel: root.channel
        autoRecordScreen:NotebookSettings.autoRecordScreen
        autoRecordDuration:NotebookSettings.autoRecordDuration
    }

    MappingReseauChecker {
        profile:root.profile
        mappingSetting:root.profile.mappingSetting
    }

    Hotkey {
        profile:root.profile
    }

    Page {
        anchors.fill: parent
        focus: true
        Rectangle {
            anchors.fill: parent
            color:"black"
        }

        ColumnLayout {
            anchors.fill: parent
            ToolBar {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                RowLayout {
                    anchors.fill: parent
                    spacing: 1
                    Control {
                        Layout.preferredHeight: 45
                        Layout.preferredWidth: 45
                        Rectangle {
                            anchors.fill: parent
                            radius:20
                            anchors.margins: 3
                            Rectangle {
                                anchors.centerIn: parent
                                color:"white"
                                radius:90
                                border.color: "#777777"
                                border.width:1
                                Image {
                                    height:25
                                    width:25
                                    anchors.centerIn: parent
                                    source: ProfileManager.currentUser.sex===User.Man? "qrc:/assets/images/man.png": "qrc:/assets/images/woman.png"
                                }
                            }

                        }

                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("List of surgery")

                        TapHandler {
                            enabled: root.profile.type !== Profile.SNAPSHOT
                            onTapped:drawer.open()
                        }
                    }

                    StatePane {
                        id:statePane
                        Layout.fillHeight: true
                        Layout.topMargin: 6
                        Layout.bottomMargin: 6
                        Layout.rightMargin: 6
                        Layout.leftMargin: 0
                        Layout.preferredWidth: 130
                        enabled: !root.combined.training
                        state: root.profile.state
                        visible: root.profile.type !== Profile.SNAPSHOT
                        property bool stateReproduce:true
                        property bool stateMelt:true
                        onAcceped: {
                            root.profile.state = statePane.state;
                            if (stateReproduce && root.profile.state === Profile.Reproduce) {
                                notebookDataSource.autoRecord("Start modeling")
                                stateReproduce = false;
                            } else if (stateMelt && root.profile.state === Profile.Melt) {
                                notebookDataSource.autoRecord("End modeling")
                                stateMelt = false;
                            }
                        }
                        property int profileType:root.profile.type
                        onProfileTypeChanged: {
                            if (statePane.profileType === Halve.PROFILE_SNAPSHOT) {
                                statePane.state = root.profile.state = Profile.Melt;
                            }
                        }
                    }

                    ToolSeparator {}
                    RowLayout {
                        spacing: 1

                        ToolButton {
                            Layout.margins: 3
                            icon {
                                source: "qrc:/assets/images/ep.png"
                                height: 30
                                width:30
                            }
                            onClicked: {
                                windowSettingPopup.toggleDockWidget(dockCatheterRepertoryPage);
                                dockCatheterRepertoryPage.moveToCenter();
                            }
                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Catheter setting")
                        }

                        


                        ToolButton {
                            Layout.margins: 3
                            icon {
                                source: "qrc:/assets/images/casket.png"
                                height: 30
                                width:30
                            }
                            onClicked: {
                                windowSettingPopup.toggleDockWidget(dockCasketView);
                                dockCasketView.moveToCenter();
                            }

                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Catheter case")
                        }

                        ToolButton {
                            id:btnCentercode
                            Layout.margins: 3
                            icon {
                                source: "qrc:/assets/images/centercode.svg"
                                height: 30
                                width:30
                            }
                            visible: root.profile.type !== Profile.SNAPSHOT

                            ConfirmDialog {
                                id: resetCenterPointConfirmDialog
                                text: qsTr("Confirm that you want to reset the center point location?")
                                prompt: qsTr("Resetting the center point removes all of the model and mapping data")
                                onAccepted: {
                                    root.combined.resetCenterPoint();
                                    notebookDataSource.autoRecord("CPC " + new Date().toLocaleString(locale, "hh:mm:ss"));
                                }
                            }

                            Menu {
                                id: centerPopup
                                y: parent.height
                                contentItem:Pane {
                                    focusPolicy: Qt.ClickFocus

                                    RowLayout {
                                        anchors.fill: parent
                                        CheckBox {
                                            text:qsTr("Use the back for reference")
                                            Layout.fillHeight: true
                                            Layout.fillWidth: true
                                            checked: root.profile.useBackReference
                                            onCheckedChanged: {
                                                root.profile.useBackReference = checked;
                                            }
                                        }
                                    }
                                }
                                onAboutToHide: root.profile.save()
                            }

                            TapHandler {
                                acceptedButtons: Qt.RightButton|Qt.LeftButton
                                onTapped:function(eventPoint, button) {
                                    if (button === Qt.RightButton) {
                                        centerPopup.open()
                                    } else {
                                       resetCenterPointConfirmDialog.open()
                                    }
                                }
                            }

                            enabled: root.profile.catheterAlined && root.combined.reproductCatheterStatus === Halve.TrackStatus_Valid && root.combined.state === ChannelReplica.State_Tracking
                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Reset center point")
                        }
                        ToolButton {
                            id:btnTraining
                            icon {
                                source: "qrc:/assets/images/train.svg"
                                height: 30
                                width:30
                            }
                            enabled: root.profile.catheterAlined && root.combined.state === ChannelReplica.State_Tracking
                            visible: root.profile.channelMode === Halve.CHANNELMODE_BLEND &&  root.profile.type !== Profile.SNAPSHOT

                            onClicked: trainConfirmDialog.open()
                            ConfirmDialog {
                                id: trainConfirmDialog
                                text: root.combined.training?qsTr("Are you sure you want to complete the training?"):qsTr("Are you sure you want to retrain?")
                                prompt: qsTr("This will clear the model and mapping data")
                                onAccepted: {
                                    root.combined.training = !root.combined.training;
                                    statePane.state = root.profile.state = root.combined.training ? Profile.Reproduce : Profile.Melt;
                                }
                            }
                            background: Rectangle {
                                implicitWidth: 20
                                implicitHeight: 20
                                visible: btnTraining.down || btnTraining.visualFocus || (btnTraining.enabled && btnTraining.hovered) || root.combined.training
                                color: btnTraining.down? "#46607c": btnTraining.enabled && btnTraining.hovered?"#464646":"#3c3c3c"
                                radius: 2
                                Rectangle {
                                    id: btnAnimatedRectangle
                                    x: 1; y: 1
                                    width: parent.width - 2
                                    height: parent.height - 2
                                    border.color: Fusion.innerContrastLine
                                    color: "transparent"
                                    visible: root.combined.training
                                    radius: 2
                                    SequentialAnimation {
                                        running: root.combined.training
                                        loops: Animation.Infinite
                                        ColorAnimation {
                                            target: btnAnimatedRectangle
                                            property: "color"
                                            from: "transparent"
                                            to: "red"
                                            duration: 2000
                                        }
                                        ColorAnimation {
                                            target: btnAnimatedRectangle
                                            property: "color"
                                            from: "red"
                                            to: "transparent"
                                            duration: 2000
                                        }
                                    }
                                }
                            }
                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Start training")

                        }

                        ToolButton {
                            id:sensorSettingButton
                            Layout.margins: 3
                            icon {
                                source: "qrc:/assets/images/sensor.png"
                                height: 30
                                width:30
                            }
                            onClicked: sensorSettingPopup.open()

                            Menu {
                                id: sensorSettingPopup
                                y: parent.height
                                contentItem:Pane {
                                    focusPolicy: Qt.ClickFocus
                                    PantSettingPane{
                                        anchors.fill: parent
                                        profile:root.profile
                                        breathSurvey:currentBreathSurvey
                                        combined:root.combined
                                        notebookDataSource:notebookDataSource
                                    }
                                }
                                onAboutToHide: root.profile.save()
                            }
                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Sensor Settings")
                        }

                        ToolSeparator {}


                        ToolButton {
                            icon {
                                source: "qrc:/assets/images/picsave.png"
                                height: 30
                                width:30
                            }
                            Layout.margins: 3
                            action: Action {
                                id: saveImageAction
                                shortcut: "F3"

                                onTriggered: {
                                    albumModel.screenshot();
                                    Qt.createComponent("FlashScreen.qml").createObject(root);
                                }
                            }
                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Save the picture")
                        }

                        ToolButton {
                            id:videoSaveButton
                            Layout.margins: 3
                            action: Action {
                                id: recordVideoAction
                                shortcut: "F4"
                                onTriggered: {
                                    if (albumModel.screenGrapRunning) {
                                        albumModel.stopScreenGrap();
                                    } else {
                                        albumModel.startScreenGrap();
                                    }
                                }
                            }
                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Save the video")
                            icon {
                                source: "qrc:/assets/images/videosave.png"
                                height: 30
                                width:30
                            }
                            Rectangle {
                                id:videoSaveButtonState
                                color:"red"
                                width: 10
                                height: 10
                                radius:90
                                anchors.bottom: parent.bottom
                                anchors.right: parent.right
                                anchors.bottomMargin: 3
                                anchors.rightMargin: 3
                                visible: albumModel.screenGrapRunning
                                SequentialAnimation {
                                    id: animation
                                    loops:Animation.Infinite
                                    running: albumModel.screenGrapRunning
                                    PropertyAnimation {
                                        target: videoSaveButtonState
                                        property: "opacity"
                                        from: 1.0
                                        to: 0.0
                                        duration: 700
                                    }
                                    PropertyAnimation {
                                        target: videoSaveButtonState
                                        property: "opacity"
                                        from: 0.0
                                        to: 1.0
                                        duration: 700
                                    }
                                }
                            }

                        }
                        ToolButton {
                            icon {
                                source: "qrc:/assets/images/album.png"
                                height: 30
                                width:30
                            }
                            Layout.margins: 3
                            onClicked:  {
                                root.StackView.view.push("AlbumPage.qml", {model:albumModel,profile:root.profile});
                            }
                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Album")
                        }
                        ToolSeparator {}
                    }
                    ToolButton {
                        Layout.margins: 3
                        icon {
                            source: "qrc:/assets/images/window_manage.svg"
                            height: 30
                            width:30
                        }
                        onClicked: windowSettingPopup.open()
                        Menu {
                            id: windowSettingPopup
                            y: parent.height

                            function toggleDockWidget(dw) {
                                if (dw.isOpen) {
                                    dw.close();
                                } else {
                                    dw.show();
                                }
                            }

                            contentItem:Pane {
                                focusPolicy: Qt.ClickFocus
                                ColumnLayout {
                                    anchors.fill: parent
                                    MenuItem {
                                        text:qsTr("Imaging window 1")
                                        checked: dockStageLeftView.isOpen
                                        checkable: true
                                        onTriggered: {
                                            windowSettingPopup.toggleDockWidget(dockStageLeftView);
                                        }
                                    }
                                    MenuItem {
                                        text:qsTr("Imaging window 2")
                                        checked: dockStageRightView.isOpen
                                        checkable: true
                                        onTriggered: {
                                            windowSettingPopup.toggleDockWidget(dockStageRightView);
                                        }
                                    }

                                    MenuItem {
                                        text:qsTr("Catheter window")
                                        checked: dockCatheterView.isOpen
                                        checkable: true
                                        onTriggered: {
                                            windowSettingPopup.toggleDockWidget(dockCatheterView);
                                        }
                                    }

                                    MenuItem {
                                        text:qsTr("Mapping window")
                                        checked: dockMeltView.isOpen
                                        checkable: true
                                        onTriggered: {
                                            windowSettingPopup.toggleDockWidget(dockMeltView);
                                        }
                                    }
                                    MenuItem {
                                        text:qsTr("Ablation window")
                                        checked: dockDissolveView.isOpen
                                        checkable: true
                                        onTriggered: {
                                            windowSettingPopup.toggleDockWidget(dockDissolveView);
                                        }
                                    }

                                    MenuItem {
                                        text:qsTr("Channel signal window")
                                        checked: dockOscillograpView.isOpen
                                        checkable: true
                                        onTriggered: {
                                            windowSettingPopup.toggleDockWidget(dockOscillograpView);
                                        }
                                    }

                                    MenuItem {
                                        text:qsTr("Mapping list window")
                                        checked: dockMappingView.isOpen
                                        checkable: true
                                        onTriggered: {
                                            windowSettingPopup.toggleDockWidget(dockMappingView);
                                        }
                                    }

                                    MenuItem {
                                        text:qsTr("Pulse window")
                                        checked: dockPulseView.isOpen
                                        checkable: true
                                        onTriggered: {
                                            windowSettingPopup.toggleDockWidget(dockPulseView);
                                        }
                                    }
                                    MenuItem {
                                        text:qsTr("Breathe window")
                                        checked: dockBreatheView.isOpen
                                        checkable: true
                                        onTriggered: {
                                            windowSettingPopup.toggleDockWidget(dockBreatheView);
                                        }
                                    }

                                    MenuItem {
                                        text:qsTr("Model list window")
                                        checked: dockReseauView.isOpen
                                        checkable: true
                                        action: Action {
                                            onTriggered: {
                                                windowSettingPopup.toggleDockWidget(dockReseauView);
                                            }
                                        }
                                    }

                                    MenuSeparator { Layout.fillWidth: true}
                                    MenuItem {
                                        text: qsTr("Full screen")
                                        action: Action {
                                            shortcut:"Ctrl+Shift+F11"
                                            onTriggered: {
                                                InterfaceSettings.fullScreen = !InterfaceSettings.fullScreen
                                            }
                                        }
                                        icon {
                                            source: "qrc:/assets/images/fullscreen.svg"
                                        }
                                    }
                                    MenuItem {
                                        text: qsTr("Default layout")
                                        onTriggered: {
                                            // layoutSaver.save()
                                            layoutSaver.setLayout(":/layout/default.json");
                                        }
                                        icon {
                                            source: "qrc:/assets/images/default_layout.svg"
                                        }
                                    }
                                }
                            }
                        }
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Window management")

                    }

                    ToolButton {
                        icon {
                            source: "qrc:/assets/images/terminal.svg"
                            height: 30
                            width:30
                        }
                        Layout.margins: 3
                        onClicked: terminalPopup.open()
                        visible: root.profile.type !== Profile.SNAPSHOT
                        TerminalMenu {
                            id: terminalPopup
                            y: parent.height
                            dockingArea: dockingArea
                            profile:root.profile
                        }
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Terminal")
                    }

                    ToolButton {
                        id:btnSettings
                        Layout.margins: 3
                        icon {
                            source: "qrc:/assets/images/settings.png"
                            height: 30
                            width:30
                        }
                        visible: root.profile.type !== Profile.SNAPSHOT
                        onClicked:  {
                            windowSettingPopup.toggleDockWidget(dockSettingView);
                            dockSettingView.moveToCenter();
                        }
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Settings")

                    }

                    ToolButton {
                        icon {
                            source: "qrc:/assets/images/power.png"
                            height: 30
                            width:30
                        }
                        visible: root.profile.type !== Profile.SNAPSHOT
                        onClicked:  {
                            StartoverState.exit(0);
                        }
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Return to User List")
                    }

                    ToolSeparator {}

                    ToolButton {
                        id:reviewButton
                        icon {
                            source: "qrc:/assets/images/review.svg"
                            height: 30
                            width:30
                        }
                        visible: root.profile.type === Profile.SNAPSHOT
                        onClicked:  {
                            root.profile.close();
                        }
                        SequentialAnimation {
                            id: reviewAnimation
                            loops:Animation.Infinite
                            running: reviewButton.visible
                            PropertyAnimation {
                                target: reviewButton
                                property: "opacity"
                                from: 1.0
                                to: 0.0
                                duration: 1000
                            }
                            PropertyAnimation {
                                target: reviewButton
                                property: "opacity"
                                from: 0.0
                                to: 1.0
                                duration: 1000
                            }
                        }
                        hoverEnabled: true
                        ToolTip.visible: hovered
                        ToolTip.text: qsTr("Exit review")
                    }
                    Item {
                        id:toolbarSpace
                        Layout.fillWidth: true
                        onWidthChanged: {
                            alarmPane.x = toolbarSpace.x + toolbarSpace.width / 2 - alarmPane.width / 2
                        }

                    }

                    RowLayout {
                        Layout.fillHeight: true
                        spacing:10
                        Layout.rightMargin: 5

                        ToolButton {
                            id:autoOmnidistanceRecordScreenButton
                            icon {
                                source: "qrc:/assets/images/screen_record.svg"
                                height: 25
                                width:25
                            }
                            property bool autoOmnidistanceRecordScreen: NotebookSettings.autoOmnidistanceRecordScreen
                            visible:autoOmnidistanceRecordScreen
                            onAutoOmnidistanceRecordScreenChanged: omnidistanceScreenRecord()

                            SequentialAnimation {
                                loops:Animation.Infinite
                                running: NotebookSettings.autoOmnidistanceRecordScreen
                                PropertyAnimation {
                                    target: autoOmnidistanceRecordScreenButton
                                    property: "opacity"
                                    from: 1.0
                                    to: 0.0
                                    duration: 1000
                                }
                                PropertyAnimation {
                                    target: autoOmnidistanceRecordScreenButton
                                    property: "opacity"
                                    from: 0.0
                                    to: 1.0
                                    duration: 1000
                                }
                            }
                            function omnidistanceScreenRecord() {
                                if (NotebookSettings.autoOmnidistanceRecordScreen) {
                                    albumModel.startScreenRecord();
                                } else {
                                    albumModel.stopScreenRecord();
                                }
                            }
                            Component.onCompleted: omnidistanceScreenRecord()

                            hoverEnabled: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("Screen recording")
                        }
                        Row {
                            visible:notebookDataSource.snapping
                            Label {
                                id:snappingCountdown
                                text:"2323"
                                anchors.verticalCenter: parent.verticalCenter
                                font.bold: true
                                font.pixelSize: 14
                                property int countdownNumber: 60
                                Timer {
                                    interval: 1000;
                                    running: notebookDataSource.snapping;
                                    onRunningChanged: {
                                        snappingCountdown.countdownNumber = 60;
                                    }
                                    repeat: true
                                    triggeredOnStart:true
                                    onTriggered: {
                                        snappingCountdown.text =--snappingCountdown.countdownNumber
                                        if (snappingCountdown.countdownNumber === 0) {
                                            if(notebookDataSource.snapping) {
                                                notebookDataSource.snapshot(new Date().getTime());
                                            }
                                        }
                                    }
                                }
                            }

                            ToolButton {
                                id:snappingButton
                                icon {
                                    source: "qrc:/assets/images/snapping.svg"
                                    height: 25
                                    width:25
                                }

                                SequentialAnimation {
                                    id: snappingAnimation
                                    loops:Animation.Infinite
                                    running: notebookDataSource.snapping
                                    PropertyAnimation {
                                        target: snappingButton
                                        property: "opacity"
                                        from: 1.0
                                        to: 0.0
                                        duration: 1000
                                    }
                                    PropertyAnimation {
                                        target: snappingButton
                                        property: "opacity"
                                        from: 0.0
                                        to: 1.0
                                        duration: 1000
                                    }
                                }
                                hoverEnabled: true
                                ToolTip.visible: hovered
                                ToolTip.text: qsTr("Snapping")
                            }
                        }


                        Label {
                            font.pixelSize: 17
                            font.bold: true
                            id:currentTimeLabel
                            Timer {
                                interval: 1000;
                                running: true;
                                repeat: true
                                triggeredOnStart:true
                                onTriggered: parent.text = new Date().toLocaleString(locale, "yyyy-MM-dd hh:mm:ss")
                            }
                        }

                        CatheterTrackWidget{
                            id:catheterTrackWidget
                            height: 35
                            width: 35
                            profile:root.profile
                            combined:root.combined
                            breathSurvey:currentBreathSurvey
                            consultPrecision:AppSettings.consultPrecision
                            rate: AppSettings.catheterTrackRate

                            ChannelModeModel {
                                id:channelModeModel
                            }

                            DeviceStateLight {
                                id:deviceStateLight
                                anchors.fill: parent
                                state: root.combined.state
                                channelMode:root.profile.channelMode
                                hoverEnabled: true
                                ToolTip.visible: hovered
                                ToolTip.text: channelModeModel.getTextByValue(root.profile.channelMode)
                            }

                            Component.onCompleted: {
                                catheterTrackWidget.init()
                            }

                            onPant0Changed: {
                                notebookDataSource.autoRecord("PPC " + new Date().toLocaleString(locale, "hh:mm:ss"));
                            }
                            Menu {
                                id: channelModeMenu
                                y:catheterTrackWidget.height

                                contentItem:Pane {
                                    focusPolicy: Qt.ClickFocus
                                    ColumnLayout{
                                        anchors.fill: parent
                                        Repeater {
                                            model:channelModeModel
                                            MenuItem {
                                                required property int index
                                                required property var modelData
                                                text: modelData.text
                                                highlighted:true
                                                font.bold: modelData.value === root.profile.channelMode
                                                onTriggered: {
                                                    root.profile.channelMode = modelData.value
                                                }
                                            }
                                        }
                                    }
                                }

                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.AllButtons
                                onClicked: (mouse)=>{
                                    if ((mouse.button === Qt.RightButton) && root.profile.type === Halve.PROFILE_TEMP) {
                                        channelModeMenu.open()
                                    }
                                }
                                onDoubleClicked: {
                                    root.channel.launchWatch()
                                }
                            }
                        }



                    }
                }
            }
            KDDW.DockingArea {
                id:dockingArea
                Layout.fillWidth: true
                Layout.fillHeight: true
                focus: true
                uniqueName: "mainWindow"


                KDDW.DockWidget {
                    id: dockSettingView
                    uniqueName: "dockSettingView"
                    title:qsTr("Setting window")
                    isFloating: true
                    focus: true
                    options:KDDW.KDDockWidgets.DockWidgetOption_NotDockable
                    SettingsPage {
                        id:settingsPage
                        anchors.fill: parent
                        visibleBackBtn:false
                    }
                }

                KDDW.DockWidget {
                    id: dockStageLeftView
                    uniqueName: "dockStageLeftView"
                    title:qsTr("Imaging")
                    focus: true
                    StageView {
                        anchors.fill: parent
                        profile:root.profile
                        obscurity:currentObscurity
                        azimuthModel:azimuthModel
                        combined:root.combined
                        channel: root.channel
                        scalarModel:currentScalarModel
                        stageScalar:currentStageScalar
                        options:root.profile.stageSettings.getOptions(dockStageLeftView.uniqueName)
                        reseauListModel:reseauListModel
                    }
                }
                KDDW.DockWidget {
                    id: dockStageRightView
                    uniqueName: "dockStageRightView"
                    title:qsTr("Imaging")
                    focus: true
                    StageView {
                        anchors.fill: parent
                        profile:root.profile
                        obscurity:currentObscurity
                        azimuthModel:azimuthModel
                        scalarModel:currentScalarModel
                        stageScalar:currentStageScalar
                        combined:root.combined
                        channel: root.channel
                        reseauListModel:reseauListModel
                        options:root.profile.stageSettings.getOptions(dockStageRightView.uniqueName)
                    }
                }
                KDDW.DockWidget {
                    id: dockOscillograpView
                    uniqueName: "dockOscillograpView"
                    title:qsTr("Channel signal")
                    focus: true
                    OscillograpView {
                        id:oscillograpView
                        profile:root.profile
                        anchors.fill: parent
                        channel: root.channel
                        breathSurvey:currentBreathSurvey
                        catheterListModel:currentCatheterListModel
                    }
                    onIsFloatingChanged: {
                        oscillograpView.reinitChartList();
                    }
                }
                KDDW.DockWidget {
                    id: dockMeltView
                    uniqueName: "dockMeltView"
                    title:qsTr("Melt window")
                    focus: true
                    MeltView {
                        id:meltView
                        anchors.fill: parent
                        profile:root.profile
                        combined:root.combined
                        channel:root.channel
                        scalarModel:currentScalarModel
                        breathSurvey:currentBreathSurvey
                        catheterListModel:currentCatheterListModel
                        mappingPointGroupListModel:currentMappingPointGroupListModel
                    }
                    onIsFloatingChanged: {
                        meltView.reinitChartList();
                    }
                }
                KDDW.DockWidget {
                    id: dockPulseView
                    uniqueName: "dockPulseView"
                    title:qsTr("Pulse window")
                    focus: true
                    PulseView {
                        anchors.fill: parent
                        profile:root.profile
                        channel: root.channel
                        catheterListModel:currentCatheterListModel
                    }
                }

                KDDW.DockWidget {
                    id: dockBreatheView
                    uniqueName: "dockBreatheView"
                    title:qsTr("Breathe window")
                    isFloating: true
                    focus: true
                    BreatheView {
                        anchors.fill: parent
                        breathSurvey:currentBreathSurvey
                        profile:root.profile
                    }
                }

                KDDW.DockWidget {
                    id: dockCasketView
                    uniqueName: "dockCasketView"
                    title:qsTr("Casket")
                    isFloating: true
                    focus: true
                    options:KDDW.KDDockWidgets.DockWidgetOption_NotDockable
                    CasketView {
                        anchors.fill: parent
                        profile:root.profile
                    }
                }
                KDDW.DockWidget {
                    id: dockCatheterRepertoryPage
                    uniqueName: "dockCatheterRepertoryPage"
                    title:qsTr("Repertory")
                    isFloating: true
                    focus: true
                    options:KDDW.KDDockWidgets.DockWidgetOption_NotDockable
                    CatheterRepertoryPage {
                        anchors.fill: parent
                        profile:root.profile
                    }
                }
                KDDW.DockWidget {
                    id: dockMappingView
                    uniqueName: "dockMappingView"
                    title:qsTr("List of calibration")
                    focus: true
                    MappingView {
                        anchors.fill: parent
                        profile:root.profile
                        mappingPointGroupListModel:currentMappingPointGroupListModel
                    }
                }
                KDDW.DockWidget {
                    id: dockDissolveView
                    uniqueName: "dockDissolveView"
                    title:qsTr("Ablation list")
                    focus: true
                    DissolveView {
                        anchors.fill: parent
                        combined:root.combined
                        profile:root.profile
                        breathSurvey:currentBreathSurvey
                    }
                }
                KDDW.DockWidget {
                    id: dockCatheterView
                    uniqueName: "dockCatheterView"
                    title:qsTr("Catheter")
                    focus: true
                    CatheterView {
                        anchors.fill: parent
                        profile:root.profile
                        model:catheterTableModel
                    }
                }
                KDDW.DockWidget {
                    id: dockReseauView
                    uniqueName: "dockReseauView"
                    title:qsTr("Model list")
                    focus: true
                    ReseauView {
                        anchors.fill: parent
                        profile:root.profile
                        combined:root.combined
                        reseauListModel:reseauListModel
                    }
                }


                KDDW.DockWidget {
                    id: dockNoteBookView
                    uniqueName: "dockNoteBookView"
                    title:qsTr("Notebook")
                    isFloating: true
                    focus: true
                    NotebookView {
                        anchors.fill: parent
                        profile:root.profile
                        source:notebookDataSource
                    }
                }
                KDDW.DockWidget {
                    id: dockVideoPlayerView
                    uniqueName: "dockVideoPlayerView"
                    title:qsTr("Player")
                    isFloating: true
                    focus: true
                    onIsOpenChanged: {
                        if (dockVideoPlayerView.isOpen) {
                            mediaPlayerView.play();
                        } else {
                            mediaPlayerView.stop();
                            mediaPlayerView.source = "";
                        }
                    }
                    MediaPlayerView {
                        id: mediaPlayerView
                        anchors.fill: parent
                    }
                }
                KDDW.DockWidget {
                    id: dockAlbumPictureView
                    uniqueName: "dockAlbumPictureView"
                    title:qsTr("Picture")
                    isFloating: true
                    focus: true
                    AlbumPictureView {
                        id: albumPictureView
                        anchors.fill: parent
                    }
                }
                Component.onCompleted: {
                    dockingArea.addDockWidget(dockStageLeftView, KDDW.KDDockWidgets.Location_OnLeft);
                    dockingArea.addDockWidget(dockStageRightView, KDDW.KDDockWidgets.Location_OnRight);
                    dockingArea.addDockWidget(dockOscillograpView, KDDW.KDDockWidgets.Location_OnBottom);
                    dockingArea.addDockWidget(dockMeltView, KDDW.KDDockWidgets.Location_OnRight, dockOscillograpView);
                    dockingArea.addDockWidget(dockCatheterView, KDDW.KDDockWidgets.Location_OnRight);
                    dockCatheterView.addDockWidgetAsTab(dockMappingView);
                    dockCatheterView.addDockWidgetAsTab(dockDissolveView);
                    dockCatheterView.addDockWidgetAsTab(dockReseauView);
                    dockCatheterView.addDockWidgetAsTab(dockNoteBookView);
                    dockCatheterView.setAsCurrentTab();
                    dockingArea.addDockWidget(dockPulseView, KDDW.KDDockWidgets.Location_OnBottom, dockDissolveView);
                    layoutSaver.restore();
                    notebookDataSource.autoRecord("Start study");
                    root.profile.increaseRef();
                }
                Component.onDestruction:{
                    if (root.profile.type !== Profile.SNAPSHOT) {
                        layoutSaver.save();
                    }
                    root.profile.decreasecRef();
                }
            }
        }
        Pane {
            id:alarmPane
            anchors.topMargin: 0
            width: Math.min(500, toolbarSpace.width)
            background: Item{}
            RowLayout {
                anchors.fill: parent
                ToolButton {
                    Layout.alignment: Qt.AlignLeft|Qt.AlignTop
                    icon {
                        source: "qrc:/assets/images/fold.svg"
                        height: 15
                        width:15
                    }
                    onClicked: {
                        alarmPaneView.loop = !alarmPaneView.loop;
                    }
                    visible:alarmPaneView.count > 1
                }
                AlarmPaneView {
                    id:alarmPaneView
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    profile: root.profile
                    interval:4000
                    backgroundOpacity:0.8
                }
            }
        }
    }

    Connections {
        target: root.profile
        function onDissolved() {
            dockDissolveView.setAsCurrentTab();
        }

        function onCurrentMappingPointGroupChanged(){
            dockMappingView.setAsCurrentTab();
        }

        function onActiveDockWidget(name){
            switch(name) {
            case "dockMappingView": {
                dockMappingView.setAsCurrentTab();
                break;
            }
            case "dockDissolveView": {
                dockDissolveView.setAsCurrentTab();
                break;
            }
            }
        }

        function onPlayVideo(fileName) {
            mediaPlayerView.source = albumModel.getFilePath(fileName)
            windowSettingPopup.toggleDockWidget(dockVideoPlayerView);
        }

        function onViewPicture(fileName) {
            albumPictureView.source = albumModel.getFilePath(fileName)
            windowSettingPopup.toggleDockWidget(dockAlbumPictureView);
        }
    }
    Connections {
        target: root.profile.mappingPointsDb
        function onAdded() {
            dockMappingView.setAsCurrentTab();
        }
    }


    ScalarModel {
        id:currentScalarModel
        profile: root.profile
        meltSetting:root.profile.meltSetting
    }

    LayoutSaver {
        id: layoutSaver
        profile: root.profile
    }

    Timer {
        interval: 1000 * 60 * NotebookSettings.autoAlternationRecordScreenInterval;
        running: NotebookSettings.autoAlternationRecordScreen;
        repeat: true
        triggeredOnStart:false
        onTriggered: {
            var name = "IAR " + new Date().toLocaleString(locale, "hh:mm:ss")
            notebookDataSource.autoRecord(name);
        }
    }
}
