import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts
import Halve

Pane {
    id:root
    required property var combined

    ColumnLayout {
        anchors.fill: parent
        Layout.fillWidth: true
         GroupBox {
            Layout.fillWidth: true
            ColumnLayout {
                anchors.fill: parent
                RowLayout {
                    Label {
                        text:"Device: "
                    }
                    Label {
                        text:combined.host
                    }
                }
                RowLayout {
                    Label {
                        text:"Status: "
                    }
                    Label {
                        id:statusTip
                        state:combined.state
                        states: [
                            State {
                                name: ChannelReplica.State_Shutdown
                                PropertyChanges {
                                    statusTip {
                                       text:"Shutdown (" + combined.state + ")"
                                       color: "gray"
                                    }
                                }
                            },
                            State {
                                name: ChannelReplica.State_Ready
                                PropertyChanges {
                                    statusTip {
                                       text:"Ready (" + combined.state + ")"
                                       color: "lightblue"
                                    }
                                }
                            },
                            State {
                                name: ChannelReplica.State_Connected
                                PropertyChanges {
                                    statusTip {
                                       text:"Connected (" + combined.state + ")"
                                       color: "white"
                                    }
                                }
                            },
                            State {
                                name: ChannelReplica.State_Disconect
                                PropertyChanges {
                                    statusTip {
                                       text:"Disconect (" + combined.state + ")"
                                       color: "red"
                                    }
                                }
                            },
                            State {
                                name: ChannelReplica.State_Tracking
                                PropertyChanges {
                                    statusTip {
                                       text:"Tracking (" + combined.state + ")"
                                       color: "green"
                                    }
                                }
                            },
                            State {
                                name: ChannelReplica.State_ConnectFailed
                                PropertyChanges {
                                    statusTip {
                                       text:"ConnectFailed (" + combined.state + ")"
                                        color: "red"
                                    }
                                }
                            },
                            State {
                                name: ChannelReplica.State_Connecting
                                PropertyChanges {
                                    statusTip {
                                       text:"Connecting (" + combined.state + ")"
                                        color: "green"
                                    }
                                }
                            }
                        ]
                    }
                }
            }

         }
    }
}
