// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import Halve

ListModel {
    id:listModel
    ListElement {
        text: qsTr("Paired bipolar")
        value:Halve.PAIR
    }
    ListElement {
        text: qsTr("All bipolar")
        value:Halve.BOTH
    }
    ListElement {
        text: qsTr("Full monopolar")
        value:Halve.SINGLE
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
