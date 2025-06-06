// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtMultimedia

Item {
    id: root
    implicitHeight: 40

    required property MediaPlayer mediaPlayer
    property alias isMediaSliderPressed: mediaSlider.pressed
    property alias showSeeker: showSeekerAnim
    property alias hideSeeker: hideSeekerAnim

    function getTime(time : int) {
        const h = Math.floor(time / 3600000).toString()
        const m = Math.floor(time / 60000).toString()
        const s = Math.floor(time / 1000 - m * 60).toString()
        return `${h.padStart(2,'0')}:${m.padStart(2,'0')}:${s.padStart(2, '0')}`
    }

    RowLayout {
        anchors.fill: root
        anchors.leftMargin: 10
        anchors.rightMargin: 10

        Label {
            id: mediaTime
            color: "#FFFFFF"
            font.bold: true
            text: root.getTime(root.mediaPlayer.position)
        }

        CustomSlider {
            id: mediaSlider
            backgroundColor:  "white"
            backgroundOpacity: 0.8
            enabled: root.mediaPlayer.seekable
            to: 1.0
            value: root.mediaPlayer.position / root.mediaPlayer.duration

            Layout.fillWidth: true

            onMoved: root.mediaPlayer.setPosition(value * root.mediaPlayer.duration)
        }

        Label {
            id: durationTime
            color: "#FFFFFF"
            font.bold: true
            text: root.getTime(root.mediaPlayer.duration)
        }
    }

    ParallelAnimation {
        id: hideSeekerAnim
        NumberAnimation {
            target: root
            properties: "opacity"
            to: 0
            duration: 1000
            easing.type: Easing.InOutQuad
        }
        NumberAnimation {
            target: root
            properties: "anchors.bottomMargin"
            to: -root.height
            duration: 1000
            easing.type: Easing.InOutQuad
        }
    }

    ParallelAnimation {
        id: showSeekerAnim
        PropertyAnimation {
            target: root
            properties: "opacity"
            to: 1
            duration: 1000
            easing.type: Easing.InOutQuad
        }
        PropertyAnimation {
            target: root
            properties: "anchors.bottomMargin"
            to: 0
            duration: 500
            easing.type: Easing.InOutQuad
        }
    }
}
