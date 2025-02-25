// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtQuick.Controls.Fusion
import QtQuick.Controls.Fusion.impl

T.CheckBox {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    padding: 0
    spacing: 0

    indicator: Rectangle {
         implicitWidth: 15
         implicitHeight: 15
         x: control.leftPadding
         y: parent.height / 2 - height / 2
         radius: 2
         border.color:  "#0078d4"
         color:"#161620"

         opacity: enabled ? 1.0 : 0.2
         Rectangle {
             width: 9
             height: 9
             x: 3
             y: 3
             radius: 1
             // opacity: control.checked?0.0:1.0
             color: "#0078d4"
             visible: control.checked
         }
     }

    contentItem: Text {
        leftPadding: control.indicator && !control.mirrored ? control.indicator.width + 4 : 0
        rightPadding: control.indicator && control.mirrored ? control.indicator.width + control.spacing : 0
        text: control.text
        font: control.font
        color: control.palette.windowText
        elide: Text.ElideRight
        opacity: enabled ? 1.0 : 0.2

        verticalAlignment: Text.AlignVCenter
    }
}
