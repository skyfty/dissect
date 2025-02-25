// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtQuick.Layouts

T.TabButton {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 2
    horizontalPadding: 1
    spacing: 6

    icon.width: 16
    icon.height: 16

    z: checked

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display

        icon: control.icon
        text: control.text
        font: control.font
        color: "white"
    }

    background:Item {
        implicitHeight: 21
        Column {
            anchors.fill: parent
            Rectangle {
                width:parent.width
                height:2
                color:control.checked ?"#2c5d87":"#282828"

            }
            Rectangle {
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#282828" }
                    GradientStop { position: 1.0; color: "transparent" }
                }
                width:parent.width
                height: 19

            }
        }
    }
}
