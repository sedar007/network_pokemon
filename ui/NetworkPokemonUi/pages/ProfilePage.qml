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

            node_cpp.save_node_infos(name, port, maxConn, share, download)

            // On rafraichit l'interface
            root.refreshConfig()
        }
    }

    Popup {
        id: addPeerPopup
        anchors.centerIn: parent
        width: parent.width - 48
        height: 320
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
                        console.log("Connexion à : " + peerNameInput.text + " [" + peerIpInput.text + "]")

                        node_cpp.add_peer(peerNameInput.text, peerIpInput.text)

                        addPeerPopup.close()
                    }
                }
            }
        }
    }
}
