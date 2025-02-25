import QtQuick
import QtQuick.Controls
 import QtQuick.Layouts
Column {
    id:root
    TabBar {
        id: bar
        TabButton {
            text: qsTr("Collection")
            width: Math.max(130, bar.width / 2)
        }
        TabButton {
            text: qsTr("Signals")
            width: Math.max(130, bar.width / 2)
        }
    }

    StackLayout {
        currentIndex: bar.currentIndex
        Rectangle {
            id: collectionTab
            width:200
            height: 200
            color:"red"
        }
        Rectangle {
            id: signalsTab
            width:500
            height: 500
            color:"yellow"
        }
    }
}
