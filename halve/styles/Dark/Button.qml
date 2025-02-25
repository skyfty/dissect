// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtQuick.Controls.Fusion
import QtQuick.Controls.Fusion.impl

T.Button {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 4
    spacing: 6

    icon.width: 16
    icon.height: 16
    icon.color: enabled?"white":"#626264"

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font
        color: enabled?"white":"#626264"
    }

    background: Rectangle {
        implicitWidth: 80
        implicitHeight: 24
        radius: 5

        color: control.down ? "#31609b":enabled && control.hovered?"#6493ce":"#404040";
        visible: !control.flat || control.down || control.checked || control.highlighted || control.visualFocus|| (enabled && control.hovered)
    }
}
