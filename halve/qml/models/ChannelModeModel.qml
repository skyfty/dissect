// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import Halve

ListModel {
    id:listModel
    ListElement {
        text: qsTr("Magnetic")
        value:Halve.CHANNELMODE_MAGNETIC
    }
    ListElement {
        text: qsTr("Electric")
        value:Halve.CHANNELMODE_ELECTRICAL
    }
    ListElement {
        text: qsTr("Blend")
        value:Halve.CHANNELMODE_BLEND
    }

    function getTextByValue(v) {
        for (var i = 0; i < listModel.count; i++) {
            if (listModel.get(i).value === (v)) {
                return listModel.get(i).text;
            }
        }
        return null;
    }
}
