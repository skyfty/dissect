// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Layouts
import QtQuick.Templates as T
import QtQuick.Controls.impl
import QtQuick.Controls.Fusion

T.MenuItem {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    padding: 3
    spacing: 6

    icon.width: 16
    icon.height: 16

    icon.color: enabled?"white":"#626264"
    contentItem:RowLayout {
        id:contentItemWarp
        IconLabel {
            id:iconLabel
            Layout.minimumWidth: 200
            readonly property real arrowPadding: control.subMenu && control.arrow ? control.arrow.width + control.spacing : 0
            readonly property real indicatorPadding: control.checkable && control.indicator ? control.indicator.width + control.spacing : 0
            leftPadding: !control.mirrored ? indicatorPadding : arrowPadding
            rightPadding: control.mirrored ? indicatorPadding : arrowPadding

            spacing: control.spacing
            mirrored: control.mirrored
            display: control.display
            alignment: Qt.AlignLeft

            color: enabled?(control.down || control.highlighted ? Fusion.highlightedText(control.palette) : control.palette.text):"#626264"
            icon: control.icon
            text: control.text
            font: control.font
        }
    }

    arrow: ColorImage {
        x: control.mirrored ? control.padding : control.width - width - control.padding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 22

        visible: control.subMenu
        rotation: control.mirrored ? 90 : -90
        color: control.down || control.hovered || control.highlighted ? Fusion.highlightedText(control.palette) : control.palette.text
        source: "qrc:/qt-project.org/imports/QtQuick/Controls/Fusion/images/arrow.png"
        fillMode: Image.Pad
    }

    indicator: CheckIndicator {
        x: control.mirrored ? control.width - width - control.rightPadding : control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2

        control: control
        visible: control.checkable
    }

    background: Rectangle {
        anchors.fill: parent
        // color: ;
        color:  control.hovered ? "#6493ce" : "#181818"
        visible: !control.flat || control.down || control.checked || control.highlighted || control.visualFocus|| (enabled && control.hovered)

    }
}
