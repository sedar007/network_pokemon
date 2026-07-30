import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    property int currentIndex: 0
    background: Rectangle { color: "white" }
    height: 60

    RowLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 10

        Label {
            text: currentIndex === 3 ? "Profil" : "Network Pokémon"
            font.pixelSize: 20
            font.bold: true
            Layout.fillWidth: true
            color: "black"
        }

        Rectangle {
            width: 10; height: 10
            radius: 5
            color: networkNode.is_p2p_active() ? "#2ECC71" : "#BDC3C7"
        }
        Label {
            text: networkNode.is_p2p_active() ? "P2P actif" : "P2P inactif"
            color: "#7F8C8D"
            font.pixelSize: 14
        }
    }
}
