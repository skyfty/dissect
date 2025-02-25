import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

ApplicationWindow {
    visible: true
    width: 1000
    height: 800
    id: root
    title: qsTr("Halve")
    required property var userId
    property string profileId:"test"
    readonly property int componentAmount:2
    property int componentInitCount: 0
    property bool fullScreen: InterfaceSettings.fullScreen

    Settings {
        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height
    }

    onFullScreenChanged: {
        StartoverState.fullScreen(root, root.fullScreen);
    }

    onClosing: {
        close.accepted = false
        StartoverState.exit(0);
    }
    Pane {
        anchors.fill: parent
        focusPolicy: Qt.ClickFocus
    }
    
    Connections {
        target: ProfileManager
        function onCurrentUserChanged(){
            ProfileManager.loadProfiles();
        }
        function onProfileAdded(profileData){
            stackView.replace("ProfileShifting.qml", {profileId:profileData.id});
        }
        function onCurrentProfileChanged(){
            ProfileManager.currentProfile.state = Profile.Melt;
            stackView.replace("ProfileLoader.qml");
        }
        function onProfileListFinished() {
            root.componentInitCount++
        }
    }

    Connections {
        target: ModelCache
        function onFinished(){
            root.componentInitCount++;
        }
        Component.onCompleted: ModelCache.prestrain()
    }

    ProfileListModel {
        id:profileListModel
    }

    Drawer {
        id: drawer
        width: 300
        height: root.height
        interactive: root.componentInitCount == root.componentAmount
        background:Rectangle {
            color:"#181818"
            border.width: 1
            border.color: "#242424"
        }

        ProfileDrawer{
            id:profileDrawer
            profileListModel:profileListModel

            anchors.fill: parent
            anchors.margins: 5
            onAbout: {
                drawer.close();
                aboutViewComponent.createObject(root).open();
            }

            onNewStudyClicked: {
                drawer.close();
                stackView.push("ProfileNewPage.qml");
            }
            onOpen: (idx)=>{
                var selectedId = profileListModel.data(profileListModel.index(idx, 0), ProfileListModel.Id)
                if (ProfileManager.currentProfile.id !== selectedId) {
                    drawer.close();
                    stackView.replace("ProfileShifting.qml", {profileId:selectedId});
                }
            }

            onDeleted: (idx)=>{
                var selectedId = profileListModel.data(profileListModel.index(idx, 0), ProfileListModel.Id)
                if (ProfileManager.currentProfile.id === selectedId) {
                    drawer.close();
                    stackView.replace("ProfileAmputate.qml", {profileListModel:profileListModel, index:idx});
                } else {
                    profileListModel.removeRows(idx, 1);
                }
            }
        }

    }

    StackView {
        id: stackView
        anchors.fill: parent
        pushEnter: Transition {
             PropertyAnimation {
                 property: "opacity"
                 from: 0
                 to:1
                 duration: 200
             }
         }
         pushExit: Transition {
             PropertyAnimation {
                 property: "opacity"
                 from: 1
                 to:0
                 duration: 200
             }
         }

         replaceEnter:Transition {
             PropertyAnimation {
                 property: "opacity"
                 from: 0
                 to:1
                 duration: 500
             }
         }

         replaceExit: Transition {
             PropertyAnimation {
                 property: "opacity"
                 from: 1
                 to:0
                 duration: 500
             }
         }
         popEnter: Transition {
             PropertyAnimation {
                 property: "opacity"
                 from: 0
                 to:1
                 duration: 200
             }
         }
         popExit: Transition {
             PropertyAnimation {
                 property: "opacity"
                 from: 1
                 to:0
                 duration: 200
             }
         }
        initialItem: Loadding{
            text:qsTr("Initializing...")
            to:root.componentAmount
            value:root.componentInitCount
        }
        Component.onCompleted: {
            ProfileManager.open(userId);
        }
        onCurrentItemChanged: {
            if (currentItem) {
              currentItem.forceActiveFocus()
            }
        }
    }

    Component {
        id:aboutViewComponent
        AboutView{
            id:aboutView
            anchors.centerIn: parent
        }
    }


    Component.onCompleted: {
        StartoverState.set()
    }

    onComponentInitCountChanged: {
        if (componentInitCount == componentAmount) {
            if (root.profileId == "new") {
                stackView.push("ProfileNewPage.qml");
            } else {
                ProfileManager.setCurrentProfile(root.profileId);
            }
        }
    }
}
