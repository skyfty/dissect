// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve
import Qt.labs.qmlmodels 1.0

import Qt.labs.platform  as Platform
Page {
    id: root
    required property var model
    required property var profile
    property int currentIndex: -1


    MessageTip {
        id: doneConfirmDialog
        text: qsTr("Export all done")
        anchors.centerIn: parent
    }
    AlbumExporter {
        id:albumExporter
        profile:root.profile
        onFinished: {
            doneConfirmDialog.open()
        }

    }
    focusPolicy: Qt.ClickFocus
    ColumnLayout {
        anchors.fill: parent
        ToolBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            focusPolicy: Qt.ClickFocus
            RowLayout {
                anchors.fill: parent
                spacing: 1
                ToolButton {
                    icon {
                        source: "qrc:/assets/images/back.png"
                        height: 25
                        width:25
                    }
                    onClicked: {
                        if (albumExporter.running) {
                            backConfirmDialog.open();
                        } else {
                            root.StackView.view.pop()
                        }
                    }

                    ConfirmDialog {
                        id: backConfirmDialog
                        text: qsTr("Are you sure to stop export?")
                        onAccepted: {
                            root.StackView.view.pop()
                        }
                    }
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Back")
                }

                ToolButton {
                    icon {
                        source: "qrc:/assets/images/delete.png"
                        height: 25
                        width:25
                    }
                    onClicked:  {
                        deleteConfirmDialog.open();

                    }
                    enabled: root.currentIndex !== -1
                    hoverEnabled: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Delete")

                    ConfirmDialog {
                        id: deleteConfirmDialog
                        text: qsTr("Are you sure you want to delete this?")
                        onAccepted: {
                            root.model.remove(root.currentIndex);
                            root.currentIndex = -1;
                        }
                    }


                }
                ToolButton {
                    Layout.margins: 3
                    icon {
                        source: "qrc:/assets/images/export.png"
                        height: 25
                        width:25
                    }
                    Platform.FolderDialog {
                        id: exportDialog;
                        property var exportGuids: []
                        title: qsTr("Please choose a folder");
                        onAccepted: {
                            var fileUrl = new String(exportDialog.folder);
                            albumExporter.exportToPath(fileUrl.substring(8));
                        }
                    }
                    enabled: !albumExporter.isRunning

                    onClicked:  {
                        exportDialog.open()
                    }
                    hoverEnabled: true
                }
                Item {
                    Layout.fillWidth: true
                }
            }
        }

        GridView {
            id:grid
            anchors.margins: 5
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            highlightFollowsCurrentItem:true

            model: root.model
            cellWidth: 300
            cellHeight: 300
            delegate: Item {
                id: container
                width: grid.cellWidth
                height: grid.cellHeight
                required property int index
                required property var thumbnail
                required property var type
                required property var portrait

                Rectangle {
                    id: content
                    color: "transparent"
                    anchors.fill: parent
                    anchors.margins: 20
                    radius: 10

                    Rectangle {
                        color: "#91AA9D"
                        anchors.fill: parent
                        anchors.margins: 3
                        radius: 8
                        antialiasing: true
                    }
                    Image {
                        source: thumbnail;
                        anchors.fill: parent
                        anchors.centerIn: parent
                    }

                    Image {
                        source:type === Picture.Image?"qrc:/assets/images/image.png":"qrc:/assets/images/video.png"
                        width:30
                        height: 30
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.margins: 5
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        root.currentIndex = container.index;
                        container.forceActiveFocus()
                    }
                    onDoubleClicked: {
                        if (type === Picture.Image) {
                            root.profile.viewPicture(portrait);
                        } else {
                            root.profile.playVideo(portrait);
                        }
                    }
                }

                states: State {
                    name: "active"
                    when: container.activeFocus
                    PropertyChanges {
                        content {
                            color: "#FCFFF5"
                            scale: 1.1
                        }
                    }
                }
                transitions: Transition {
                    NumberAnimation {
                        properties: "scale"
                        duration: 100
                    }
                }
            }
            focus: true
        }
    }


    Rectangle {
        visible: albumExporter.running
        anchors.centerIn: parent
        width: 500
        height: 20
        ProgressBar {
            id:progressBar
            anchors.fill: parent
            anchors.margins: 1
            value: albumExporter.progressValue
        }
        Label {
            anchors.centerIn: parent
            text:(albumExporter.progressValue * 100) + "%"
            font.bold: true
            font.pixelSize: 14
        }
    }
}
