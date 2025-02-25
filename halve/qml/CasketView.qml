
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve

Pane {
    id:root
    required property var profile

    CasketListModel{
        id:currentCasketListModel
        profile:root.profile
    }
    CasketTable{
        anchors.fill: parent
        profile:root.profile
        casketListModel:currentCasketListModel
    }
}
