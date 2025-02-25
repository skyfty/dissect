// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtQuick.Controls.Fusion
import QtQuick.Controls.Fusion.impl

T.ToolButton {
    id: control
    property int radius: 2
    property alias visibleBackground:panel.visible
    property alias visiblecolor:panel.color

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 6

    focusPolicy:Qt.NoFocus
    icon.width: 16
    icon.height: 16
    icon.color: enabled?"transparent":"#626264"

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
        id: panel
        implicitWidth: 20
        implicitHeight: 20
        visible: control.down || control.checked || control.highlighted || control.visualFocus || (enabled && control.hovered)
        color: control.down || control.checked? "#46607c": enabled && control.hovered?"#464646":"#3c3c3c"
        radius: control.radius
        Rectangle {
            x: 1; y: 1
            width: parent.width - 2
            height: parent.height - 2
            border.color: Fusion.innerContrastLine
            color: "transparent"
            radius: control.radius
        }
    }
}
