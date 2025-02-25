import QtQuick 2.0
import Halve 1.0
import QtQuick.Controls

Control {
    id:root
    property alias state:stateLight.state
    property alias channelMode:channelModeImage.state

    Rectangle {
        radius:90
        anchors.centerIn: parent
        width:parent.width
        height:parent.height
        Rectangle {
            id:stateLight
            radius:parent.radius
            anchors.centerIn: parent
            height: parent.height - 4
            width: parent.width - 4
            states: [
                State {
                    name: ChannelReplica.State_Shutdown
                    PropertyChanges {
                        stateLight {
                           color: "snow"
                        }
                    }
                },
                State {
                    name: ChannelReplica.State_Ready
                    PropertyChanges {
                        stateLight {
                           color: "lightblue"
                        }
                    }
                },
                State {
                    name: ChannelReplica.State_Connected
                    PropertyChanges {
                        stateLight {
                           color: "mediumslateblue"
                        }
                    }
                },
                State {
                    name: ChannelReplica.State_Disconect
                    PropertyChanges {
                        stateLight {
                           color: "red"
                        }
                    }
                },
                State {
                    name: ChannelReplica.State_Tracking
                    PropertyChanges {
                        stateLight {
                           color: "green"
                        }
                    }
                },
                State {
                    name: ChannelReplica.State_ConnectFailed
                    PropertyChanges {
                        stateLight {
                            color: "red"
                        }
                    }
                }
            ]
            transitions: Transition {
                 ColorAnimation { duration: 500 }
            }
        }

        ChannelModeImage {
            id:channelModeImage
            anchors.fill: parent
            anchors.margins: 3
        }
    }


}
