import QtQuick
import Halve

ListModel {
    id:listModel

    ListElement {
        text: qsTr("the electric reference position is too offset, adjust the reference catheter and continue with modeling")
        value:Halve.AN_ELECTRIC_REFERENCE_POSITION_IS_TOO_OFFSET
    }
    ListElement {
        text: qsTr("The CS catheter cartridge is not configured")
        value:Halve.AN_THE_CS_CATHETER_CARTRIDGE_IS_NOT_CONFIGURED
    }
    ListElement {
        text: qsTr("Incorrect configuration of reference catheter on chest")
        value:Halve.AN_PRETHORACIC_REFERENCE_CATHETER_CONFIGURATION_ERROR
    }
    ListElement {
        text: qsTr("The reference catheter configuration on the back is incorrect")
        value:Halve.AN_BACK_REFERENCE_CATHETER_CONFIGURATION_ERROR
    }
    ListElement {
        text: qsTr("The configuration of the modeling catheter is incorrect. Please check the catheter box and catheter configuration")
        value:Halve.AN_REPRODUCE_CATHETER_CONFIGURATION_ERROR
    }
    ListElement {
        text: qsTr("The designated port of the modeling catheter has no valid data")
        value:Halve.AN_REPRODUCE_CATHETER_NOT_RECEICE_DATA_ERROR
    }
    ListElement {
        text: qsTr("The electrode parameters of the modeling catheter are incorrect")
        value:Halve.AN_REPRODUCE_CATHETER_CONSULT_TARGET_CONFIG_ERROR
    }
    ListElement {
        text: qsTr("Modeling catheter lost")
        value:Halve.AN_REPRODUCE_CATHETER_MISSED_ERROR
    }
    ListElement {
        text: qsTr("The model catheter has not yet been configured to the catheter box")
        value:Halve.AN_REPRODUCE_CATHETER_NOT_IN_CASKET_ERROR
    }
    ListElement {
        text: qsTr("The catheter port is reused")
        value:Halve.AN_CATHETER_PORT_REUSED_ERROR
    }
    ListElement {
        text: qsTr("Please note that the storage space has exceeded the predetermined value")
        value:Halve.AN_STORAGE_FULL_ERROR
    }
    ListElement {
        text: qsTr("Modeling training ok. Please stop training and start modeling.");
        value:Halve.AN_MAGNETIC_ELECTRIC_TRAINNING_OK
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
