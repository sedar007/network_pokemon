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


    property var currentConfig: { "nodeName": "Chargement...", "port": "..." }
    property string activePeersLabel: "0/0"

    function refreshConfig() {
        var data = networkNode.get_node_infos();
        if (data && Object.keys(data).length > 0) root.currentConfig = data;
    }

    function refreshPeerStats() {
        if (myPeerModel) {
            root.activePeersLabel = myPeerModel.online_peers_count() + "/" + myPeerModel.peers_count();
        }
    }

    Component.onCompleted: {
        refreshConfig();
        refreshPeerStats();
    }

    // myPeerModel se rafraîchit déjà en tâche de fond (timer dans PeersPage, qui
    // reste instanciée par le StackLayout même hors écran) ; ce timer ne fait que
    // relire le compte à jour pour le badge "Peers Actifs".
    Timer {
        interval: 3000
        running: true; repeat: true
        onTriggered: root.refreshPeerStats()
    }

    // --- INTERFACE ---
    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth
        clip: true

        ColumnLayout {
            // Plafonné et centré sur grand écran (macOS) : un formulaire étiré
            // sur toute la largeur d'une fenêtre agrandie est difficile à lire.
            width: Math.min(parent.width, 640)
            anchors.horizontalCenter: parent.horizontalCenter
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
                            Text { text: networkNode.get_ip(); color: "#7F8C8D" }
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
                StatCard { label: "Total Pokémon"; value: myPokemonModel ? myPokemonModel.rowCount().toString() : "0"; icon: "📦" }
                StatCard { label: "Peers Actifs"; value: root.activePeersLabel; icon: "🔗" }
            }

            // Actions
            Text { text: "Actions rapides"; font.pixelSize: 20; font.weight: Font.DemiBold; Layout.topMargin: 10 }
            ActionButton { btnIcon: "➕"; btnText: "Ajouter un peer manuellement"; onClicked: addPeerPopup.open() }
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

            networkNode.save_node_infos(name, port, maxConn, share, download)

            // On rafraichit l'interface
            root.refreshConfig()
        }
    }

    Popup {
        id: addPeerPopup
        anchors.centerIn: parent
        width: Math.min(parent.width - 48, 420)
        height: 380
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        background: Rectangle {
            color: "white"
            radius: 20
            clip: true
        }

        Overlay.modal: Rectangle {
            color: "#80000000"
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            // Header Popup
            RowLayout {
                Layout.fillWidth: true
                Text {
                    text: "Ajouter un peer"; font.pixelSize: 22; font.bold: true
                }
                Item {
                    Layout.fillWidth: true
                }
                Text {
                    text: "✕"; font.pixelSize: 20; color: "#999"; TapHandler {
                        onTapped: addPeerPopup.close()
                    }
                }
            }
            Text {
                text: "Entrez les informations du nœud distant"; color: "#7F8C8D"; font.pixelSize: 14
            }

            // Champs de saisie
            ColumnLayout {
                Layout.fillWidth: true; spacing: 5
                Text {
                    text: "Nom du nœud (Optionnel)"; font.bold: true; font.pixelSize: 14
                }
                TextField {
                    id: peerNameInput
                    placeholderText: "Ex: TeamRocket-Server"
                    Layout.fillWidth: true; Layout.preferredHeight: 50
                    font.pixelSize: 16; color: "black"
                    background: Rectangle {
                        color: "#F8F9FA"; radius: 12
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true; spacing: 5
                Text {
                    text: "Adresse IP"; font.bold: true; font.pixelSize: 14
                }
                TextField {
                    id: peerIpInput
                    placeholderText: "Ex: 192.168.1.15"
                    Layout.fillWidth: true; Layout.preferredHeight: 50
                    font.pixelSize: 16; color: "black"
                    background: Rectangle {
                        color: "#F8F9FA"; radius: 12
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true; spacing: 5
                Text {
                    text: "Port (Optionnel, 49153 par défaut)"; font.bold: true; font.pixelSize: 14
                }
                TextField {
                    id: peerPortInput
                    placeholderText: "Ex: 49153"
                    Layout.fillWidth: true; Layout.preferredHeight: 50
                    font.pixelSize: 16; color: "black"
                    validator: IntValidator { bottom: 1; top: 65535 }
                    background: Rectangle {
                        color: "#F8F9FA"; radius: 12
                    }
                }
            }

            Item {
                Layout.fillHeight: true
            }

            // Boutons
            RowLayout {
                Layout.fillWidth: true; spacing: 15
                Button {
                    Layout.fillWidth: true; Layout.preferredHeight: 50
                    background: Rectangle {
                        color: "white"; border.color: "#E1E4E8"; radius: 12
                    }
                    contentItem: Text {
                        text: "Annuler"; font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: addPeerPopup.close()
                }
                Button {
                    Layout.fillWidth: true; Layout.preferredHeight: 50
                    background: Rectangle {
                        color: "#050510"; radius: 12
                    }
                    contentItem: Text {
                        text: "Connecter"; color: "white"; font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        console.log("Connexion à : " + peerNameInput.text + " [" + peerIpInput.text + ":" + peerPortInput.text + "]")

                        networkNode.add_peer(peerNameInput.text, peerIpInput.text, peerPortInput.text)

                        addPeerPopup.close()
                    }
                }
            }
        }
    }
}
