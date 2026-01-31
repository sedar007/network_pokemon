import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
import "../components" // Assurez-vous que ce chemin est correct pour vos composants
import NetworkPokemonUi

Item {
    // Il faut un Item conteneur pour que le Popup puisse se centrer par rapport à lui
    anchors.fill: parent

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth
        clip: true

        ColumnLayout {
            width: parent.width
            spacing: 20
            Layout.margins: 20
            Layout.bottomMargin: 40

            Backend {
                id: myBackend
            }

            // --- TITRE SECTION ---
            Text {
                text: "Mon nœud"
                font.pixelSize: 18; font.weight: Font.DemiBold
                Layout.leftMargin: 20; Layout.topMargin: 20
            }

            // --- 1. CARTE DU NŒUD ---
            Rectangle {
                Layout.fillWidth: true; Layout.preferredHeight: 220
                Layout.leftMargin: 20; Layout.rightMargin: 20
                radius: 16; color: "white"
                layer.enabled: true
                layer.effect: MultiEffect { shadowEnabled: true; shadowColor: "#10000000"; shadowBlur: 10; shadowVerticalOffset: 4 }

                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 20; spacing: 20

                    // Ligne du haut
                    RowLayout {
                        Layout.fillWidth: true; spacing: 15
                        Rectangle {
                            width: 50; height: 50; radius: 25; color: "#E8F0FE"
                            Text { text: "🖥️"; anchors.centerIn: parent; font.pixelSize: 24 }
                        }
                        Column {
                            Layout.fillWidth: true; spacing: 4
                            Text { text: "Pikachu-Node"; font.bold: true; font.pixelSize: 16 }
                            Text { text: myBackend.getDeviceIp(); color: "#7F8C8D"; font.pixelSize: 14 }
                            Rectangle {
                                width: 70; height: 24; radius: 12; color: "black"; anchors.topMargin: 5
                                Text { text: "En ligne"; color: "white"; font.pixelSize: 11; font.bold: true; anchors.centerIn: parent }
                            }
                        }

                        // --- LE BOUTON ENGRENAGE ---
                        Rectangle {
                            width: 40; height: 40; radius: 10; color: "#F8F9FA"; border.color: "#E1E4E8"
                            Text { text: "⚙️"; anchors.centerIn: parent }

                            // AJOUT ICI : Le clic pour ouvrir le popup
                            TapHandler {
                                onTapped: settingsPopup.open()
                            }
                        }
                    }

                    // Ligne du bas (ID et Port)
                    RowLayout {
                        Layout.fillWidth: true; spacing: 15
                        Repeater {
                            model: [ { label: "ID du nœud", val: "node-pk-7a8f" }, { label: "Port", val: "8080" } ]
                            Rectangle {
                                Layout.fillWidth: true; Layout.preferredHeight: 60; color: "#F8F9FA"; radius: 12
                                Column {
                                    anchors.centerIn: parent; width: parent.width - 30
                                    Text { text: modelData.label; color: "#7F8C8D"; font.pixelSize: 12 }
                                    Text { text: modelData.val; font.bold: true; font.pixelSize: 14; elide: Text.ElideRight; width: parent.width }
                                }
                            }
                        }
                    }
                }
            }

            // --- SUITE DU CONTENU (Stats, Stockage, etc...) ---
            Text { text: "Statistiques"; font.pixelSize: 18; font.weight: Font.DemiBold; Layout.topMargin: 10; Layout.leftMargin: 20 }

            GridLayout {
                columns: 2; columnSpacing: 15; rowSpacing: 15
                Layout.fillWidth: true; Layout.leftMargin: 20; Layout.rightMargin: 20
                // Vos StatCards ici...
                StatCard { label: "Total Pokémon"; value: "6"; icon: "📦" } // Assurez-vous d'avoir défini StatCard ou remplacez par le code complet
                StatCard { label: "Peers actifs"; value: "3/5"; icon: "👥" }
            }

            // --- BOUTONS D'ACTION ---
            // On connecte aussi le gros bouton "Paramètres" au popup
            ActionButton {
                btnIcon: "⚙️"; btnText: "Paramètres du nœud"
                onClicked: settingsPopup.open()
            }
            ActionButton { btnIcon: "+"; btnText: "Ajouter un peer manuellement" }
            ActionButton { btnText: "Voir les logs" }

            Item { height: 20; Layout.fillWidth: true }
        }
    }

    // --- LE POPUP (MODAL) ---
    Popup {
        id: settingsPopup
        anchors.centerIn: parent
        width: parent.width - 40
        height: 550 // Hauteur approximative
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        background: Rectangle {
            color: "white"
            radius: 20
            clip: true
        }

        Overlay.modal: Rectangle {
            color: "#80000000" // Fond noir semi-transparent
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            // Header Popup
            RowLayout {
                Layout.fillWidth: true
                Text { text: "Paramètres du nœud"; font.pixelSize: 20; font.bold: true }
                Item { Layout.fillWidth: true }
                Text {
                    text: "✕"; font.pixelSize: 20; color: "#999"
                    TapHandler { onTapped: settingsPopup.close() }
                }
            }
            Text { text: "Configurez les paramètres de votre nœud P2P"; color: "#7F8C8D"; font.pixelSize: 14; Layout.bottomMargin: 10 }

            // Champs de saisie
            component ConfigInput : ColumnLayout {
                property string label; property string placeholder
                Layout.fillWidth: true; spacing: 5
                Text { text: label; font.bold: true; font.pixelSize: 14 }
                TextField {
                    text: placeholder
                    Layout.fillWidth: true; Layout.preferredHeight: 45
                    background: Rectangle { color: "#F5F5F5"; radius: 8 }
                    font.pixelSize: 16
                }
            }

            ConfigInput { label: "Nom du nœud"; placeholder: "Pikachu-Node" }
            ConfigInput { label: "Port"; placeholder: "8080" } // Simulez les flèches si besoin
            ConfigInput { label: "Connexions maximales"; placeholder: "10" }

            // Switches
            component ConfigSwitch : RowLayout {
                property string title; property string subtitle; property bool isActive
                Layout.fillWidth: true; Layout.topMargin: 5
                Column {
                    Layout.fillWidth: true
                    Text { text: title; font.bold: true; font.pixelSize: 14 }
                    Text { text: subtitle; color: "#7F8C8D"; font.pixelSize: 12 }
                }
                Switch {
                    checked: isActive
                    // Personnalisation simple pour ressembler au design iOS
                    indicator: Rectangle {
                        implicitWidth: 48; implicitHeight: 26; radius: 13
                        color: parent.checked ? "black" : "#E0E0E0"
                        Rectangle {
                            x: parent.parent.checked ? parent.width - width - 2 : 2
                            y: 2; width: 22; height: 22; radius: 11; color: "white"
                            Behavior on x { NumberAnimation { duration: 100 } }
                        }
                    }
                }
            }

            ConfigSwitch { title: "Partage automatique"; subtitle: "Partager automatiquement vos Pokémon locaux"; isActive: true }
            ConfigSwitch { title: "Téléchargement automatique"; subtitle: "Télécharger automatiquement les nouvelles images"; isActive: false }

            Item { Layout.fillHeight: true }

            // Boutons Footer
            RowLayout {
                Layout.fillWidth: true; spacing: 10
                Button {
                    Layout.fillWidth: true; Layout.preferredHeight: 50
                    background: Rectangle { color: "white"; border.color: "#E1E4E8"; radius: 8 }
                    contentItem: Text { text: "Annuler"; font.bold: true; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
                    onClicked: settingsPopup.close()
                }
                Button {
                    Layout.fillWidth: true; Layout.preferredHeight: 50
                    background: Rectangle { color: "#050510"; radius: 8 }
                    contentItem: RowLayout {
                        anchors.centerIn: parent
                        Text { text: "💾"; color: "white" }
                        Text { text: "Sauvegarder"; color: "white"; font.bold: true }
                    }
                    onClicked: settingsPopup.close()
                }
            }
        }
    }
}
