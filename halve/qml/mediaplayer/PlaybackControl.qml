// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Controls.Fusion
import QtQuick.Effects
Item {
    id: root

    implicitHeight: 100

    required property MediaPlayer mediaPlayer
    readonly property int mediaPlayerState: root.mediaPlayer.playbackState

    property alias volume: audio.volume
    property alias playbackRate: rate.playbackRate

    function changeLoopMode() {
        if (root.mediaPlayer.loops === 1 && !root.isPlaylistLooped) {
            root.mediaPlayer.loops = MediaPlayer.Infinite
        } else {
            root.mediaPlayer.loops = 1
            root.isPlaylistLooped = false
        }
    }

    Item {
        anchors.fill: root

        RowLayout {
            id: playerButtons
            anchors.fill: parent

            Item {
                Layout.fillWidth: true
                Layout.minimumWidth: 40
                Layout.maximumWidth: 95
            }

            PlaybackRateControl {
                id: rate
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Item {
                Layout.fillWidth: true
            }

            RowLayout {
                id: controlButtons
                spacing: Screen.primaryOrientation === Qt.LandscapeOrientation ? 25 : 1

                Layout.alignment: Qt.AlignHCenter
                Layout.fillWidth: true


                CustomButton {
                    id: playButton
                    icon.source: "icons/Play_Icon.svg"
                    onClicked: root.mediaPlayer.play()
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Play")
                }

                CustomButton {
                    id: pausedButton
                    icon.source: "icons/Stop_Icon.svg"
                    onClicked: root.mediaPlayer.pause()
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Pause")
                }

                CustomButton {
                    id: loopButton
                    icon.source: "icons/Loop_Icon_Dark.svg"
                    visible: Screen.primaryOrientation === Qt.LandscapeOrientation
                    onClicked: root.changeLoopMode()

                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Loop playing")
                    states: [
                        State {
                            name: "noActiveLooping"
                            when: root.mediaPlayer.loops === 1 && !root.isPlaylistLooped
                            PropertyChanges {
                                loopButton.icon.source: "icons/Loop_Icon_Dark.svg"
                            }
                        },
                        State {
                            name: "singleLoop"
                            when: root.mediaPlayer.loops === MediaPlayer.Infinite
                            PropertyChanges {
                                loopButton.icon.source: "icons/Single_Loop.svg"
                            }
                        }
                    ]
                }
            }

            Item {
                Layout.fillWidth: true
            }

            AudioControl {
                id: audio
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Item {
                Layout.fillWidth: true
                Layout.minimumWidth:40
                Layout.maximumWidth: 95

            }
        }
    }

    states: [
        State {
            name: "playing"
            when: root.mediaPlayerState == MediaPlayer.PlayingState

            PropertyChanges {
                playButton.visible: false
            }
            PropertyChanges {
                pausedButton.visible: true
            }
        },
        State {
            name: "paused"
            when: root.mediaPlayerState == MediaPlayer.PausedState || root.mediaPlayerState == MediaPlayer.StoppedState

            PropertyChanges {
                playButton.visible: true
            }
            PropertyChanges {
                pausedButton.visible: false
            }
        }
    ]
}

