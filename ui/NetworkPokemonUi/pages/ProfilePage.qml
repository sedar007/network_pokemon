import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import NetworkPokemonUi

// Import du dossier components
import "../components"

Item {
    id: root
    anchors.fill: parent

    Backend { id: myBackend }
    Node { id: node_cpp}

    property var currentConfig: { "nodeName": "Chargement...", "port": "..." }

    function refreshConfig() {
        var data = node_cpp.get_node_infos();
        if (data && Object.keys(data).length > 0) root.currentConfig = data;
    }

    Component.onCompleted: refreshConfig()

    // --- INTERFACE ---
    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth
        clip: true

        ColumnLayout {
            width: parent.width
            spacing: 24
            Layout.margins: 24
            Layout.bottomMargin: 50

            Text {
                text: "Mon nœud P2P"; font.pixelSize: 28; font.weight: Font.Bold; Layout.topMargin: 20
            }

            // Carte Principale
            Rectangle {
                Layout.fillWidth: true; Layout.preferredHeight: 180
                radius: 24; color: "white"
                layer.enabled: true
                layer.effect: MultiEffect { shadowEnabled: true; shadowBlur: 20; shadowColor: "#20000000" }

                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 24; spacing: 15

                    RowLayout {
                        Layout.fillWidth: true; spacing: 15
                        Rectangle { width: 56; height: 56; radius: 28; color: "#E8F0FE"; Text { text: "⚡"; anchors.centerIn: parent; font.pixelSize: 26 } }

                        Column {
                            Layout.fillWidth: true
                            Text { text: root.currentConfig.nodeName; font.bold: true; font.pixelSize: 18 }
                            Text { text: node_cpp.get_ip(); color: "#7F8C8D" }
                        }

                        Rectangle {
                            width: 44; height: 44; radius: 14; color: "#F8F9FA"
                            Text { text: "⚙️"; anchors.centerIn: parent; font.pixelSize: 18 }
                            TapHandler { onTapped: settingsPopup.open() }
                        }
                    }

                    Rectangle { height: 1; Layout.fillWidth: true; color: "#F0F0F0" }

                    Text {
                        text: "En ligne • Port " + root.currentConfig.port
                        color: "#27AE60"; font.bold: true
                    }
                }
            }

            // Stats
            Text { text: "Aperçu global"; font.pixelSize: 20; font.weight: Font.DemiBold }
            GridLayout {
                columns: 2; columnSpacing: 16
                Layout.fillWidth: true
                StatCard { label: "Total Pokémon"; value: "152"; icon: "📦" }
                StatCard { label: "Peers Actifs"; value: "3/8"; icon: "🔗" }
            }

            // Actions
            Text { text: "Actions rapides"; font.pixelSize: 20; font.weight: Font.DemiBold; Layout.topMargin: 10 }
            ActionButton { btnIcon: "➕"; btnText: "Ajouter un peer manuellement" }
            ActionButton { btnIcon: "📜"; btnText: "Voir les journaux (Logs)" }
        }
    }

    // --- POPUP ---
    SettingsPopup {
        id: settingsPopup
        anchors.centerIn: parent

        // On passe la config actuelle au popup
        currentConfig: root.currentConfig

        // On reçoit le signal "Sauvegarder" du popup
        onRequestSave: (name, port, maxConn, share, download) => {
            console.log("Sauvegarde demandée depuis le composant Popup")

            node_cpp.save_node_infos(name, port, maxConn, share, download)

            // On rafraichit l'interface
            root.refreshConfig()
        }
    }
}
