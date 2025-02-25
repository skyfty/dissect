import QtQuick

Item {
    Rectangle {

        id: wrapper
        width: ListView.view.width
        height: contactInfo.height
        color: ListView.isCurrentItem ? "#46607c" : (row %2==0?"#2b2b2b":"#282828")

        Text {
            id: contactInfo
            text: "lsdkjf"
            color: wrapper.ListView.isCurrentItem ? "red" : "black"
        }
    }
}
