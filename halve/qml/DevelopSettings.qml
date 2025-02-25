pragma Singleton
import QtCore
import QtQuick 2.0
import Halve

Settings {
    category:"develop"
    property int controlPort: 6007
    property string fpgaAddress: "127.0.0.1"
    property int fpgaPort: 6007
}
