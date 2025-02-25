// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import Halve

ListModel {
    ListElement {
        text: qsTr("Create time (ASC)")
        value:Halve.NOTEBOOK_SORT_CRETETIME_ASC
    }
    ListElement {
        text: qsTr("Create time (DESC)")
        value:Halve.NOTEBOOK_SORT_CRETETIME_DESC
    }
    ListElement {
        text: qsTr("By category")
        value:Halve.NOTEBOOK_SORT_CATEGORY
    }
}
