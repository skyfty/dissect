// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Halve

Page {
    id: root
    property var model
    User{
        id:userData
    }

    Rectangle {
        color:"#21233a"
        anchors.fill: parent
        Image {
            source: "qrc:/assets/images/keyan.svg"
            width:700
            height: 700
            opacity:0.02
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }
        UserForm {
            id:userForm
            anchors.fill: parent
            initUserData:userData
            onFinished: {
                if (root.model.getUser(userData.idcard)) {
                    userForm.showIdCardError();
                } else {
                    root.model.append(userData, true);
                }
            }
            onCancel: {
                root.StackView.view.pop();
            }
        }

    }
    Connections {
        target: ProfileManager
        function onCurrentUserChanged(){
            root.StackView.view.pop();
            ProfileManager.startStudy();
        }
    }
}
