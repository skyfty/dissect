// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Controls.Fusion

Rectangle {
    id: indicator

    property Item control
    readonly property color pressedColor: Fusion.mergedColors(control.palette.base, control.palette.windowText, 85)
    readonly property color checkMarkColor: Qt.darker(control.palette.text, 1.2)

    implicitWidth: 15
    implicitHeight: 15

    radius: width / 2
    color: control.down ? indicator.pressedColor : control.palette.base
    border.color:  "#4772b3"
    opacity: indicator.control.enabled ? 1.0 : 0.2

    Rectangle {
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        width: parent.width / 2
        height: parent.height / 2
        radius: width / 2
        color: "#0078d4"
        visible: indicator.control.checked
    }
}
