import QtQuick
import QtQuick.Controls

TabBar {
    id: root
    height: 70
    background: Rectangle { color: "white" }

    component NavButton: TabButton {
        property string iconText
        property string labelText
        contentItem: Column {
            spacing: 5
            Text {
                text: iconText
                font.pixelSize: 24
                anchors.horizontalCenter: parent.horizontalCenter
                color: parent.parent.checked ? "#3498DB" : "#95A5A6"
            }
            Text {
                text: labelText
                font.pixelSize: 11
                anchors.horizontalCenter: parent.horizontalCenter
                color: parent.parent.checked ? "#3498DB" : "#95A5A6"
            }
        }
        background: Rectangle { color: "transparent" }
    }

    NavButton { iconText: "🖼️"; labelText: "Galerie" }
    NavButton { iconText: "🔗"; labelText: "Peers" }
    NavButton { iconText: "🔍"; labelText: "Recherche" }
    NavButton { iconText: "👤"; labelText: "Profil" }
}
