// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property real volume: volumeSlider.value / 100

    Layout.minimumWidth: 100
    Layout.maximumWidth: 200

    RowLayout {
        anchors.fill: root
        spacing: 10

        Image {
            source: "icons/Mute_Icon_Dark.svg"
        }

        CustomSlider {
            id: volumeSlider
            enabled: true
            to: 100.0
            value: 100.0

            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
        }

        Image {
            source: "icons/Volume_Icon_Dark.svg"
        }
    }
}
