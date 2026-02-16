import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import NetworkPokemonUi

Item {
    id: root

    // CONFIGURATION : Intervalle en secondes (par défaut 2 secondes)
    property int intervalSeconds: 2

    // --- LE TIMER (Rafraîchissement automatique en arrière-plan) ---
    Timer {
        id: autoRefreshTimer
        interval: root.intervalSeconds * 1000
        running: true
        repeat: true
        onTriggered: {
            // Met à jour les données silencieusement
            myPeerModel.refreshPeers()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 20

        // --- TOP BAR : Infos et Configuration du Timer ---
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            Layout.topMargin: 10
            radius: 8
            color: "white"
            border.color: "#E1E4E8"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

                Text { text: "⚖️"; font.pixelSize: 16 }

                Text {
                    text: myPeerModel.peers_count() + " peers listés"
                    color: "#666"; font.pixelSize: 14
                }

                // Espace flexible pour pousser le slider à droite
                Item { Layout.fillWidth: true }

                Text {
                    text: "Auto: " + root.intervalSeconds + "s"
                    color: "#666"; font.pixelSize: 12
                }

                Slider {
                    id: timeSlider
                    from: 1     // Minimum 1 seconde
                    to: 10      // Maximum 10 secondes
                    stepSize: 1
                    value: root.intervalSeconds
                    Layout.preferredWidth: 100

                    // Met à jour la vitesse de rafraîchissement en direct
                    onMoved: root.intervalSeconds = value
                }
            }
        }

        // --- HEADER : Titre et Bouton de rafraîchissement manuel ---
        RowLayout {
            Layout.fillWidth: true
            Text {
                text: "Serveurs connectés"
                font.pixelSize: 18; font.bold: true
            }
            Item { Layout.fillWidth: true }

            Rectangle {
                width: 110; height: 36
                radius: 18
                color: "white"
                border.color: "#E1E4E8"
                border.width: 1

                Row {
                    anchors.centerIn: parent
                    spacing: 5
                    Text {
                        id: refreshIcon
                        text: "🔄"
                        font.pixelSize: 12
                        Behavior on rotation {
                            NumberAnimation { duration: 500; easing.type: Easing.OutCubic }
                        }
                    }
                    Text { text: "Actualiser"; font.bold: true; font.pixelSize: 13 }
                }

                TapHandler {
                    onTapped: {
                        // 1. Fait tourner l'icône
                        refreshIcon.rotation += 360

                        // 2. Met à jour les données
                        myPeerModel.refreshPeers()

                        // 3. Redémarre le timer pour décaler le prochain auto-refresh
                        autoRefreshTimer.restart()
                    }
                }
            }
        }

        // --- LISTE DES SERVEURS ---
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
            height: 90

            Rectangle {
                anchors.fill: parent
                color: "white"
                radius: 12
                layer.enabled: true
                layer.effect: MultiEffect {
                    shadowEnabled: true; shadowColor: "#10000000"; shadowBlur: 10; shadowVerticalOffset: 2
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 15

                    Rectangle {
                        width: 44; height: 44; radius: 22
                        color: {
                            if (status === "online") return "#E8F5E9"
                            if (status === "sync") return "#E3F2FD"
                            return "#F5F5F5"
                        }

                        Text {
                            anchors.centerIn: parent
                            text: "🗄️"
                            font.pixelSize: 20
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4
                        Text { text: name; font.bold: true; font.pixelSize: 16 }
                        Text { text: ip; color: "#7F8C8D"; font.pixelSize: 13 }
                        RowLayout {
                            spacing: 10
                            Text { text: count; color: "#7F8C8D"; font.pixelSize: 11 }
                            Text { text: ping; color: "#7F8C8D"; font.pixelSize: 11 }
                            Text { text: lastSeen; color: "#7F8C8D"; font.pixelSize: 11 }
                        }
                    }

                    Rectangle {
                        Layout.preferredHeight: 24
                        Layout.preferredWidth: statusLabel.width + 24
                        radius: 12
                        color: {
                            if (status === "online") return "black"
                            if (status === "sync") return "white"
                            return "#F0F2F5"
                        }
                        border.color: status === "sync" ? "#E1E4E8" : "transparent"
                        border.width: status === "sync" ? 1 : 0

                        Row {
                            anchors.centerIn: parent
                            spacing: 5
                            Text {
                                text: {
                                    if (status === "online") return "📶"
                                    if (status === "sync") return "🔄"
                                    return "📡"
                                }
                                font.pixelSize: 10
                                color: status === "online" ? "white" : "black"
                            }
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
