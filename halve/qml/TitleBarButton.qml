/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

import QtQuick 2.9

Rectangle {
    id: root

    signal clicked()
    property alias imageSource: image.source

    color: "transparent"
    height: 18
    width: 18

    radius: 3


    Image {
        id: image
        anchors.centerIn: parent
        width: 16
        height: 16
        anchors {
            verticalCenterOffset: 1
            horizontalCenterOffset: 1
        }
    }

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onClicked: {
            root.clicked();
        }
    }
}
