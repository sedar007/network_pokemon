import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    property string title
    property alias checked: sw.checked // Alias pour récupérer l'état

    Layout.fillWidth: true
    spacing: 10

    Text {
        text: title
        font.pixelSize: 16
        Layout.fillWidth: true
        color: "#333"
    }

    Switch {
        id: sw
        indicator: Rectangle {
            implicitWidth: 50
            implicitHeight: 30
            radius: 15
            color: sw.checked ? "#34C759" : "#E5E5EA"
            Rectangle {
                x: sw.checked ? parent.width - width - 2 : 2
                y: 2
                width: 26
                height: 26
                radius: 13
                color: "white"
                Behavior on x { NumberAnimation { duration: 200 } }
            }
        }
    }
}
