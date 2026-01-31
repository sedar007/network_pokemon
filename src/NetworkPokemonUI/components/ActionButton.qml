import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    id: control
    property string btnIcon: ""
    property string btnText: ""

    Layout.fillWidth: true
    Layout.preferredHeight: 50
    Layout.leftMargin: 20; Layout.rightMargin: 20

    background: Rectangle {
        color: control.down ? "#F0F2F5" : "white"
        border.color: "#E1E4E8"
        radius: 8
    }

    contentItem: RowLayout {
        spacing: 10
        anchors.centerIn: parent
        Text {
            text: control.btnIcon
            visible: control.btnIcon !== ""
            font.pixelSize: 16
        }
        Text {
            text: control.btnText
            font.bold: true
            font.pixelSize: 14
            color: "black"
        }
    }
}
