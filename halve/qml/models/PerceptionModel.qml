// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import Halve

ListModel {
    ListElement {
        text: qsTr("Maximum")
        value:Halve.Max
    }
    ListElement {
        text: qsTr("Minimum")
        value:Halve.Min
    }
    ListElement {
        text: qsTr("-dVdt")
        value:Halve.PlusdVdt
    }
    ListElement {
        text: qsTr("+dVdt")
        value:Halve.MinusdVdt
    }
    ListElement {
        text: qsTr("abs dVdt")
        value:Halve.AbsdVdt
    }
    ListElement {
        text: qsTr("abs Peak")
        value:Halve.AbsPeak
    }
}
