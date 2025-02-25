import QtQuick
import QtCore
import QtQuick.Controls
import QtQuick.Window
import Halve
import QtQuick.Layouts

Image {
    id:root
    states: [
        State {
            name: Halve.CHANNELMODE_ELECTRICAL

            PropertyChanges {
                root.source:  "qrc:/assets/images/electrical.svg"
            }
        },
        State {
            name: Halve.CHANNELMODE_MAGNETIC

            PropertyChanges {
                root.source:  "qrc:/assets/images/magnetic.svg"
            }
        },
        State {
            name: Halve.CHANNELMODE_BLEND

            PropertyChanges {
                root.source: "qrc:/assets/images/blend.svg"
            }
        }
    ]
}
