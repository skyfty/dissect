import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

import Qt.labs.platform  as Platform
import Halve 1.0
import com.kdab.dockwidgets 2.0 as KDDW


Menu {
    id: root
    required property var dockingArea
    required property var profile

    FpgaTest {
        id:fpgaTest
    }

    contentItem:Pane {
        focusPolicy: Qt.ClickFocus
        ColumnLayout {
            anchors.fill: parent
            MenuItem {
                text:qsTr("Terminal")
                onTriggered: {
                    root.dockingArea.toggleDockWidget("qrc:/qml/TerminalView.qml",qsTr("Terminal"));
                }
                icon {
                    source: "qrc:/assets/images/consoler.svg"
                }
            }

            MenuSeparator {
                contentItem: Rectangle{
                    implicitHeight: 1
                    color: "white"
                }
            }
            MenuItem {
                id:importBtn
                text:qsTr("Import")
                onTriggered: {
                    fileImportDialog.open();
                }
                icon {
                    source: "qrc:/assets/images/import.png"
                }
                enabled: !meshImporter.running

                background: Item {
                    Rectangle {
                        anchors.fill: parent
                        color: "#181818";
                    }
                    ProgressBar {
                        id:exportProgressBar
                        anchors.fill: parent
                        indeterminate:true
                        visible: meshImporter.running
                    }
                }
                Platform.FileDialog {
                     id: fileImportDialog
                     title: qsTr("Please choose a mesh file")
                     nameFilters: ["Suported Types (*.ply *.vtp *.obj *.stl *.vtk)"]
                     fileMode:Platform.FileDialog.OpenFile
                     selectedNameFilter.index: 1
                     onAccepted: {
                         var fileUrl = new String(fileImportDialog.currentFile);
                         meshImporter.open(fileUrl.substring(8));
                     }
                }

                MeshImporter {
                    id:meshImporter
                    profile:root.profile
                }
            }

            MenuSeparator {
                contentItem: Rectangle{
                    implicitHeight: 1
                    color: "white"
                }
            }
            MenuItem {
                text:qsTr("s6008")
                checked: fpgaTest.s6008
                checkable: true
                onTriggered: {
                    fpgaTest.s6008 = checked;
                }
            }

            MenuItem {
                text:qsTr("The catheter window")
                checked: fpgaTest.s6009
                checkable: true
                onTriggered: {
                    fpgaTest.s6009 = checked;
                }
            }

        }
    }
}
