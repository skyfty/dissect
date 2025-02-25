// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Window
import QtQuick.Controls.Fusion
import QtMultimedia
import QtQuick.Effects

Item {
    id: root
    property alias source: mediaPlayer.source
    function play() {
        mediaPlayer.play();
    }
    function stop() {
        mediaPlayer.stop();
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged: {
            if (!seeker.opacity) {
                seeker.showSeeker.start()
            } else {
                timer.restart()
            }
        }
    }

    Timer {
        id: timer
        interval: 3000
        onTriggered: {
            if (!seeker.isMediaSliderPressed) {
                seeker.hideSeeker.start()
            } else {
                timer.restart()
            }
        }
    }

    ErrorPopup {
        id: errorPopup
    }

    MediaPlayer {
        id: mediaPlayer
        playbackRate: playbackControl.playbackRate
        videoOutput: videoOutput
        audioOutput: AudioOutput {
            id: audio
            volume: playbackControl.volume
        }
        onErrorOccurred: {
            errorPopup.errorMsg = mediaPlayer.errorString
            errorPopup.open()
        }
        onTracksChanged: {
        }

        onMediaStatusChanged: {

        }
    }

    VideoOutput {
        id: videoOutput

        anchors.top: parent.top
        anchors.bottom: playbackControl.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin:  20
        anchors.rightMargin: 20
        visible: mediaPlayer.hasVideo
    }

    Image {
        id: defaultCoverArt
        anchors.horizontalCenter: videoOutput.horizontalCenter
        anchors.verticalCenter: videoOutput.verticalCenter
        visible: !videoOutput.visible && mediaPlayer.hasAudio
        source: "icons/Default_CoverArt.svg"
    }

    Rectangle {
        id: background
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: seeker.opacity ? seeker.top : playbackControl.top
        color: "#09102B"
        opacity: videoOutput.fullScreen ? 0.75 : 0.5
    }

    Image {
        id: shadow
        source: `icons/Shadow.png`
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    PlaybackSeekControl {
        id: seeker
        anchors.left: videoOutput.left
        anchors.right: videoOutput.right
        anchors.bottom: playbackControl.top
        mediaPlayer: mediaPlayer
    }

    PlaybackControl {
        id: playbackControl

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        mediaPlayer: mediaPlayer
    }


    ParallelAnimation {
        id: hideControls

        NumberAnimation {
            targets: [playbackControl, seeker, background, shadow]
            property: "opacity"
            to: 0
            duration: 1000
            easing.type: Easing.InOutQuad
        }
        NumberAnimation {
            target: playbackControl
            property: "anchors.bottomMargin"
            to: -playbackControl.height - seeker.height
            duration: 1000
            easing.type: Easing.InOutQuad
        }
    }

    ParallelAnimation {
        id: showControls

        NumberAnimation {
            targets: [playbackControl, seeker, shadow]
            property: "opacity"
            to: 1
            duration: 1000
            easing.type: Easing.InOutQuad
        }
        NumberAnimation {
            target: background
            property: "opacity"
            to: 0.5
            duration: 1000
            easing.type: Easing.InOutQuad
        }
        NumberAnimation {
            target: playbackControl
            property: "anchors.bottomMargin"
            to: 0
            duration: 1000
            easing.type: Easing.InOutQuad
        }
    }

    Component.onCompleted: {
        mediaPlayer.play()
    }
}
