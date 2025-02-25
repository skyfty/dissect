import QtQuick
import Halve

Item {
    TreeModel {
        id: treemodel
        roles: ["display"]
        TreeElement{
            property string display: ProfileManager.currentUser.fullName
            TreeElement{
                property string display: "Study-1"
            }
        }
    }
    TreeView {
        id:profileTreeView
        anchors.fill: parent
        model: treemodel
        anchors.margins: 5

        delegate: Item {
            id: treeDelegate

            implicitWidth: padding + label.x + label.implicitWidth + padding
            implicitHeight: label.implicitHeight * 1.5

            readonly property real indent: 10
            readonly property real padding: 5

            // Assigned to by TreeView:
            required property TreeView treeView
            required property bool isTreeNode
            required property bool expanded
            required property int hasChildren
            required property int depth

            TapHandler {
                onTapped: treeView.toggleExpanded(row)
            }

            Text {
                id: indicator
                visible: treeDelegate.isTreeNode && treeDelegate.hasChildren
                x: padding + (treeDelegate.depth * treeDelegate.indent)
                anchors.verticalCenter: label.verticalCenter
                text: "▸"
                color:"white"
                font.pixelSize: 14
                font.bold: true
                rotation: treeDelegate.expanded ? 90 : 0
            }

            Text {
                id: label
                x: padding + (treeDelegate.isTreeNode ? (treeDelegate.depth + 1) * treeDelegate.indent : 0)
                width: treeDelegate.width - treeDelegate.padding - x
                clip: true
                text: model.display
                color:"white"
                font.bold: true
                font.pixelSize: 14
            }
        }

        Component.onCompleted: {
            profileTreeView.expand(0)
        }
    }
}
