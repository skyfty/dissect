pragma Singleton
import QtCore
import QtQuick 2.0
import Halve
import QtQml

Settings {
    category:"deploy"
    property int combinedInterval: 50
    property bool keepSave: false

    property real basis:325.42
    property real ecgBasis:271.18
    property int basisUnit:10
    property int samplingRate:2000
    property int uploadRate:64
    property int channelPort: 6006
    property int oscillograpScaleplateStep: 10

    property int channelChartListLabelWidth: 75

    property int defaultSurveyor: 4
    property int breathSamplingRate : 20
    property bool breathSamplingFilter : false
    property real breathSamplingValidLimit:0.5
    property real consultPrecision : 5.0
    property real breathGatingRatio : 30
    property real breathScaleplateMax : 300
    property real electricDisplacement:4.00
    property var electricCenter:Qt.vector3d(0.0, 0.0, 20.0)

    property int magnetismTrainRate : 5
    property var coefficient:Qt.vector3d(100.0, 100.0, 100.0)
    property int catheterTrackRate : 3
    property int storageThreshold:10
    property string defaultPulseElectrodeId:"bodysurface.bodysurface.ecg2"
    property string defaultUserPath:""

}
