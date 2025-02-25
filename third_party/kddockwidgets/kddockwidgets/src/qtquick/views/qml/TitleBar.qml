/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

import QtQuick 2.9

/**
  * @brief Implements a Title Bar which looks like its QWidgets counterpart.
  *
  * For custom title bars please derive directly from TitleBarBase.qml and not TitleBar.qml,
  * as there's not much to reuse in this one.
  */
TitleBarBase {
    id: root

    // These two are just for unit-tests
    readonly property QtObject floatButton: floatButton
    readonly property QtObject closeButton: closeButton

    color: "#2e2f30"
    heightWhenVisible: 20
    Row {
         Image {
              source: "qrc:/img/dock-float.png"
         }

        Text {
            id: title
            text: root.title
                    color:"white"
            anchors {
                left: parent ? parent.left : undefined
                leftMargin: 5
                verticalCenter: parent.verticalCenter
            }
        }
     }

    Row {
        anchors {
            verticalCenter: parent ? parent.verticalCenter : undefined
            right: parent ? parent.right : undefined
            topMargin: 5
            bottomMargin: 5
            rightMargin: 5
        }

        TitleBarButton {
            id: minimizeButton
            visible: root.minimizeButtonVisible
            imageSource: "qrc:/img/min.png";
            onClicked: {
                root.minimizeButtonClicked();
            }
        }

        TitleBarButton {
            id: floatButton
            visible: root.floatButtonVisible
            imageSource: "qrc:/img/dock-float.png"
            onClicked: {
                root.floatButtonClicked();
            }
        }

        TitleBarButton {
            id: maximizeButton
            visible: root.maximizeButtonVisible
            imageSource: root.maximizeUsesRestoreIcon ? "qrc:/img/dock-float.png" : "qrc:/img/max.png";
            onClicked: {
                root.maximizeButtonClicked();
            }
        }

        TitleBarButton {
            id: closeButton
            enabled: root.closeButtonEnabled
            imageSource: "qrc:/img/close.png"
            onClicked: {
                root.closeButtonClicked();
            }
        }
    }
}
