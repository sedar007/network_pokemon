import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

Rectangle {
    property string label
    property string value
    property string icon

    Layout.fillWidth: true; Layout.preferredHeight: 100
    radius: 16; color: "white"
    layer.enabled: true
    layer.effect: MultiEffect { shadowEnabled: true; shadowColor: "#10000000"; shadowBlur: 10; shadowVerticalOffset: 2 }

    Column {
        anchors.left: parent.left; anchors.top: parent.top; anchors.margins: 15; spacing: 10
        Row {
            spacing: 5
            Text { text: icon; font.pixelSize: 14 }
            Text { text: label; color: "#7F8C8D"; font.pixelSize: 14 }
        }
        Text { text: value; font.pixelSize: 28; font.weight: Font.Medium }
    }
}
