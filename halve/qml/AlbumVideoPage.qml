import QtQuick

import QtQuick.Controls
import QtQuick.Layouts
import Halve
import "mediaplayer"

Page {
    id: root
    required property var index
    required property var model
    header:ToolBar {
        RowLayout {
            anchors.fill: parent
            spacing: 1

            ToolButton {
                icon {
                    source: "qrc:/assets/images/back.png"
                    height: 25
                    width:25
                }
                onClicked:root.StackView.view.pop()
            }

            ToolButton {
                icon {
                    source: "qrc:/assets/images/delete.png"
                    height: 25
                    width:25
                }
                onClicked:  {
                    model.remove(index)
                    root.StackView.view.pop();
                }
                hoverEnabled: true
                ToolTip.visible: hovered
                ToolTip.text: qsTr("删除")
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }

    MediaPlayerView {
        id: mediaplayer
        anchors.fill: parent
        source: model.data(model.index(index, 0), AlbumModel.Portrait)
    }
}
