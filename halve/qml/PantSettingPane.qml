
import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve 1.0


RowLayout {
    id:root
    required property var profile

    property var breathOptions:profile.breathOptions
    required property var breathSurvey
    required property var combined
    required property var notebookDataSource

    BreathOptionPane {
        profile:root.profile
        breathSurvey:root.breathSurvey
        combined:root.combined
        notebookDataSource:root.notebookDataSource
        Layout.fillHeight: true
        Layout.fillWidth: true
    }

}
