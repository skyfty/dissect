pragma Singleton
import QtCore
import QtQuick 2.0
import Halve

Settings {
    category:"notebook"
    property bool autoRecordScreen: false
    property int autoRecordDuration:11
    property bool autoOmnidistanceRecordScreen: false
    property bool autoAlternationRecordScreen: false
    property int autoAlternationRecordScreenInterval: 15

}
