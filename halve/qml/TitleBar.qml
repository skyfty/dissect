/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2020-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sergio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

import QtQuick 2.6
import QtQuick.Layouts
import QtQuick.Controls
// Will be moved to a plugin in the future, if there's enough demand
import "qrc:/kddockwidgets/qtquick/views/qml/" as KDDW

KDDW.TitleBarBase {
    id: root
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#282828" }
        GradientStop { position: 1.0; color: "transparent" }
    }
    heightWhenVisible: 24

    Text {
        id: title
        text: root.title
        color:"white"
        font.pixelSize: 12
        anchors {
            left: parent ? parent.left : undefined
            leftMargin: 3
            verticalCenter: parent.verticalCenter
        }
    }



    ToolBar {
        anchors {
            verticalCenter: parent ? parent.verticalCenter : undefined
            right: parent ? parent.right : undefined
        }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 2
            spacing: 0


            ToolButton {
                id: moreButton
                visible: root.moreButtonVisible
                icon {
                    source: "qrc:/assets/images/window_more.png"
                    height: 12
                    width:12
                }
                onClicked: {
                    root.moreButtonClicked();
                }

            }

            ToolButton {
                id: floatButton
                icon {
                    source: root.isFloating?"qrc:/assets/images/window_inject.png":"qrc:/assets/images/window_float.png"
                    height: 12
                    width:12
                }
                onClicked: {
                    root.floatButtonClicked();
                }

            }

            ToolButton {
                id: maximizeButton
                icon {
                    source: root.maximizeUsesRestoreIcon ? "qrc:/assets/images/window_restore.png" : "qrc:/assets/images/window_maximize.png";
                    height: 12
                    width:12
                }
                visible: root.isFloating
                onClicked: {
                    root.maximizeButtonClicked();
                }

            }

            ToolButton {
                id: closeButton
                enabled: root.closeButtonEnabled
                icon {
                    source: "qrc:/assets/images/window_close.png"
                    height: 12
                    width:12
                }
                onClicked: {
                    root.closeButtonClicked();
                }
            }
        }
    }
}
