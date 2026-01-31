import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import NetworkPokemonUi

Item {
    id: root

    // 2. Instanciation du modèle C++
        PeerModel {
            id: myPeerModel
        }

    // Contenu Principal
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 20

        // 1. Barre de résumé (Top Bar)
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            Layout.topMargin: 10
            radius: 8
            color: "white"
            // Bordure légère
            border.color: "#E1E4E8"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10
                Text { text: "⚖️"; font.pixelSize: 16 }
                Text { text: "3 peers en ligne sur 5"; color: "#666"; font.pixelSize: 14 }
            }
        }

        // 2. Titre + Bouton Actualiser
        RowLayout {
                    Layout.fillWidth: true
                    Text {
                        text: "Serveurs connectés"
                        font.pixelSize: 18; font.bold: true
                    }
                    Item { Layout.fillWidth: true }

                    // --- BOUTON ACTUALISER MODIFIÉ ---
                    Rectangle {
                        width: 110; height: 36
                        radius: 18
                        color: "white"
                        border.color: "#E1E4E8"
                        border.width: 1

                        Row {
                            anchors.centerIn: parent
                            spacing: 5

                            // L'icône qui va tourner
                            Text {
                                id: refreshIcon
                                text: "🔄"
                                font.pixelSize: 12

                                // Animation de rotation
                                Behavior on rotation {
                                    NumberAnimation { duration: 500; easing.type: Easing.OutCubic }
                                }
                            }

                            Text { text: "Actualiser"; font.bold: true; font.pixelSize: 13 }
                        }

                        TapHandler {
                            onTapped: {
                                // 1. Animation visuelle (tourne de 360 degrés à chaque clic)
                                refreshIcon.rotation += 360

                                // 2. Appel au Backend C++
                                myPeerModel.refreshPeers()
                            }
                        }
                    }
                }

        // 3. Liste des Serveurs (ListView)
        ListView {
            id: peerList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 15

            model: myPeerModel
            delegate: peerDelegate
        }
    }

    // --- COMPOSANT D'AFFICHAGE (Delegate) ---
    Component {
        id: peerDelegate
        Item {
            width: peerList.width
            height: 90 // Hauteur de la carte

            Rectangle {
                anchors.fill: parent
                color: "white"
                radius: 12

                // Ombre portée
                layer.enabled: true
                layer.effect: MultiEffect {
                    shadowEnabled: true; shadowColor: "#10000000"; shadowBlur: 10; shadowVerticalOffset: 2
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 15

                    // 1. Icône Serveur (Couleur dynamique)
                    Rectangle {
                        width: 44; height: 44; radius: 22
                        // Couleur de fond selon le status
                        color: {
                            if (status === "online") return "#E8F5E9" // Vert clair
                            if (status === "sync") return "#E3F2FD"   // Bleu clair
                            return "#F5F5F5"                          // Gris clair
                        }

                        Text {
                            anchors.centerIn: parent
                            text: "🗄️"
                            font.pixelSize: 20
                        }
                    }

                    // 2. Infos principales
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Text { text: name; font.bold: true; font.pixelSize: 16 }
                        Text { text: ip; color: "#7F8C8D"; font.pixelSize: 13 }

                        // Ligne Stats
                        RowLayout {
                            spacing: 10
                            Text { text: count; color: "#7F8C8D"; font.pixelSize: 11 }
                            Text { text: ping; color: "#7F8C8D"; font.pixelSize: 11 }
                            Text { text: lastSeen; color: "#7F8C8D"; font.pixelSize: 11 }
                        }
                    }

                    // 3. Badge de Status (Design dynamique)
                    Rectangle {
                        Layout.preferredHeight: 24
                        Layout.preferredWidth: statusLabel.width + 24
                        radius: 12

                        // Couleur du badge
                        color: {
                            if (status === "online") return "black"
                            if (status === "sync") return "white"
                            return "#F0F2F5" // Gris
                        }
                        border.color: status === "sync" ? "#E1E4E8" : "transparent"
                        border.width: status === "sync" ? 1 : 0

                        Row {
                            anchors.centerIn: parent
                            spacing: 5

                            // Icône status
                            Text {
                                text: {
                                    if (status === "online") return "📶"
                                    if (status === "sync") return "🔄"
                                    return "📡"
                                }
                                font.pixelSize: 10
                                color: status === "online" ? "white" : "black"
                            }

                            // Texte status
                            Text {
                                id: statusLabel
                                text: {
                                    if (status === "online") return "En ligne"
                                    if (status === "sync") return "Synchronisation"
                                    return "Hors ligne"
                                }
                                font.pixelSize: 11
                                font.bold: true
                                color: status === "online" ? "white" : "black"
                            }
                        }
                    }
                }
            }
        }
    }
}
