import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Item {
    id: root
    // Propriétés requises par le GridView
    property string name
    property string pNumber
    property string type
    property string size
    property string imgUrl

    width: GridView.view.cellWidth - 10
    height: GridView.view.cellHeight - 10

    Rectangle {
        anchors.fill: parent; color: "white"; radius: 12
        layer.enabled: true
        layer.effect: MultiEffect { shadowEnabled: true; shadowColor: "#20000000"; shadowBlur: 1.0; shadowVerticalOffset: 2 }

        ColumnLayout {
            anchors.fill: parent; spacing: 0
            Rectangle {
                Layout.fillWidth: true; Layout.preferredHeight: 160; color: "#F8F9FA"; radius: 12; clip: true
                Rectangle { anchors.bottom: parent.bottom; width: parent.width; height: 12; color: "#F8F9FA" }
                Image { source: root.imgUrl; anchors.fill: parent; anchors.margins: 10; fillMode: Image.PreserveAspectFit }
            }
            ColumnLayout {
                Layout.fillWidth: true; Layout.margins: 12; spacing: 5
                RowLayout {
                    Layout.fillWidth: true
                    Text { text: root.name; font.bold: true; font.pixelSize: 16; Layout.fillWidth: true }
                    Text { text: root.pNumber; color: "#95A5A6"; font.pixelSize: 14 }
                }
                Row {
                    spacing: 5
                    Repeater {
                        model: root.type.split(',')
                        Rectangle {
                            width: typeLabel.width + 16; height: 24; radius: 12; color: "#F0F2F5"; border.color: "#E1E4E8"
                            Text { id: typeLabel; text: modelData; anchors.centerIn: parent; font.pixelSize: 11; color: "#333" }
                        }
                    }
                }
                Text { text: root.size; color: "#95A5A6"; font.pixelSize: 12; Layout.topMargin: 5 }
                Item { Layout.fillHeight: true }
            }
        }
    }
}
