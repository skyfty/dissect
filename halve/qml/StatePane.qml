
import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve 1.0
import com.kdab.dockwidgets 2.0 as KDDW
import Qt.labs.platform as Platform
import "models"

Item {
    id:root
    required property var state
    signal acceped();

    Rectangle {
        anchors.fill: parent
        radius:5
        color:"#dddddd"
        ListView {
            id: listView
            anchors.fill: parent
            orientation:ListView.Horizontal
            anchors.margins: 2
            focus: true
            clip: true
            spacing:0

            flickableDirection: Flickable.HorizontalFlick
            model: PerformStateModel{}
            delegate: Control {
                width: listView.width / 2
                height: ListView.view.height
                required property var text
                required property var index
                required property var icon
                required property var value
                property var isCurrentItem:ListView.isCurrentItem

                Image {
                    source: icon
                    anchors.centerIn:parent
                    anchors.margins: 3
                    height:33
                    width:33
                }

                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: text

                TapHandler {
                    onTapped: {
                        root.state= value;
                        root.acceped();
                    }
                }

            }
            highlight: Rectangle {
                width: listView.width / 2
                height: ListView.view.height
                radius: 5
                color: "#6a6767";
                x: listView.currentItem.x
                Behavior on x{
                    SpringAnimation {
                        spring: 2
                        damping: 0.2
                    }
                }
            }
            highlightFollowsCurrentItem:false
            states: [
                State {
                    name: Profile.Reproduce
                    PropertyChanges {
                        listView.currentIndex : 0
                    }
                },

                State {
                    name: Profile.Melt
                    PropertyChanges {
                        listView.currentIndex : 1
                    }
                }
            ]
            state: root.state
        }

    }

}
