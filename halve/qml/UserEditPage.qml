// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve

Page {
    id: root
    property var model
    property User userData
    User{
        id:tempUserData
        Component.onCompleted: {
            tempUserData.deepCopy(userData)
            loader.sourceComponent = userFormComponent;
            // loader.active = true;
        }
    }
    Rectangle {
        color:"#21233a"
        anchors.fill: parent
        Image {
            source: "qrc:/assets/images/yanjiu.svg"
            width:700
            height: 700
            opacity:0.02
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }

        Component {
            id:userFormComponent
            UserForm {
                id:userForm
                anchors.fill: parent
                initUserData:tempUserData
                onFinished: {
                    var usr = root.model.getUser(tempUserData.idcard);
                    if (usr !== null && userData.id !== usr.id) {
                        userForm.showIdCardError();
                    } else {
                        userData.deepCopy(tempUserData)
                        model.update(userData);
                    }

                }
                onCancel: {
                    root.StackView.view.pop();
                }
            }
        }

        Loader {
            id:loader
            anchors.fill: parent
        }
    }

    Connections {
        target: model
        onDataChanged:{
            root.StackView.view.pop();
        }
    }
}
