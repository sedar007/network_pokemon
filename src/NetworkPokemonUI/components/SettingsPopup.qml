import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: popup

    // Propriété pour recevoir la config actuelle depuis Main.qml
    property var currentConfig: ({})

    // Signal émis quand l'utilisateur clique sur Sauvegarder
    signal requestSave(string name, string port, string maxConn, bool share, bool download)

    width: Math.min(parent.width - 40, 420)
    height: 600
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    // À l'ouverture, on remplit les champs
    onOpened: {
        if(currentConfig) {
            inputName.textValue = currentConfig.nodeName || ""
            inputPort.textValue = currentConfig.port || ""
            inputMaxConn.textValue = currentConfig.maxConnections || ""
            switchShare.checked = currentConfig.autoShare || false
            switchDownload.checked = currentConfig.autoDownload || false
        }
    }

    background: Rectangle { color: "white"; radius: 24; clip: true }
    Overlay.modal: Rectangle { color: "#66000000" }

    ColumnLayout {
        anchors.fill: parent; anchors.margins: 24; spacing: 20

        // Header
        RowLayout {
            Layout.fillWidth: true
            Text { text: "Paramètres"; font.pixelSize: 24; font.bold: true }
            Item { Layout.fillWidth: true }
            Rectangle {
                width: 36; height: 36; radius: 18; color: "#F2F2F7"
                Text { text: "✕"; anchors.centerIn: parent; color: "#8E8E93" }
                TapHandler { onTapped: popup.close() }
            }
        }

        // Formulaire
        ScrollView {
            Layout.fillWidth: true; Layout.fillHeight: true
            contentWidth: availableWidth; clip: true

            ColumnLayout {
                width: parent.width; spacing: 18

                // On utilise nos composants créés précédemment
                // QML les trouve automatiquement car ils sont dans le même dossier
                ConfigInput { id: inputName; label: "Nom du nœud"; placeholder: "Ex: Sacha-Node" }
                ConfigInput { id: inputPort; label: "Port d'écoute (TCP)"; placeholder: "8080" }
                ConfigInput { id: inputMaxConn; label: "Connexions Max"; placeholder: "10" }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#E5E5EA"; Layout.margins: 5 }

                ConfigSwitch { id: switchShare; title: "Partage P2P automatique" }
                ConfigSwitch { id: switchDownload; title: "Téléchargement auto" }
            }
        }

        // Bouton
        Button {
            Layout.fillWidth: true; Layout.preferredHeight: 56
            background: Rectangle {
                color: "black"; radius: 16
                opacity: parent.down ? 0.8 : 1.0
            }
            contentItem: RowLayout {
                anchors.centerIn: parent
                Text { text: "💾"; color: "white"; font.pixelSize: 18 }
                Text { text: "Enregistrer"; color: "white"; font.bold: true; font.pixelSize: 17 }
            }

            onClicked: {
                // On émet le signal vers Main.qml
                popup.requestSave(
                    inputName.textValue,
                    inputPort.textValue,
                    inputMaxConn.textValue,
                    switchShare.checked,
                    switchDownload.checked
                )
                popup.close()
            }
        }
    }
}
