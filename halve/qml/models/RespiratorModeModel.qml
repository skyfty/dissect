// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import Halve

ListModel {
    ListElement {
        text: qsTr("None")
        value:Halve.RES_MODE_NOTHING
    }
    ListElement {
        text: qsTr("Breathing gate control")
        value:Halve.RES_MODE_GATING
    }
    ListElement {
        text: qsTr("Respiratory compensation")
        value:Halve.RES_MODE_ADAPTIVE
    }
}
