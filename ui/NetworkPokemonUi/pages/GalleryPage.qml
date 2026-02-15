import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "../components"

Item {
    id: root

    property string tempFilePath: ""

    // Propriétés pour l'élément sélectionné et affiché en grand
    property int currentIndex: -1
    property var currentPokemon: ({})

    // --- 1. POPUP DE CONFIRMATION DE SUPPRESSION ---
    MessageDialog {
        id: deleteConfirmDialog
        title: "Confirmation de suppression"
        text: (root.currentPokemon && root.currentPokemon.isMine)
            ? "⚠️ ATTENTION : Vous êtes le propriétaire.\n\nSupprimer cette image l'effacera DU RÉSEAU pour tout le monde.\n\nÊtes-vous sûr ?"
            : "Voulez-vous supprimer cette image de votre vue locale ?"

        buttons: MessageDialog.Yes | MessageDialog.No

        onButtonClicked: function(button, role) {
            if (button === MessageDialog.Yes) {
                if (root.currentIndex !== -1) {
                    myPokemonModel.removePokemon(root.currentIndex)
                    fullScreenView.close()
                }
            }
        }
    }

    // --- 2. POPUP POUR ENREGISTRER (CORRIGÉ) ---
    FileDialog {
        id: saveFileDialog
        title: "Enregistrer l'image sous..."
        fileMode: FileDialog.SaveFile
        // SUPPRESSION DE LA LIGNE 'currentName' QUI FAISAIT CRASHER L'APP
        nameFilters: ["Images PNG (*.png)", "Images JPG (*.jpg)"]

        onAccepted: {
            // Appel C++ pour sauvegarder
            // selectedFile contient le chemin complet choisi par l'utilisateur
            myPokemonModel.savePokemon(root.currentIndex, selectedFile)
        }
    }

    // --- 3. SÉLECTEUR D'IMAGE (POUR AJOUT) ---
    FileDialog {
        id: addFileDialog
        title: "Choisir une image"
        nameFilters: ["Images (*.png *.jpg *.jpeg)"]
        onAccepted: {
            root.tempFilePath = selectedFile
            nameInput.text = ""
            nameDialog.open()
        }
    }

    // --- 4. DIALOGUE NOM (POUR AJOUT) ---
    Dialog {
        id: nameDialog
        title: "Ajouter un Pokémon"
        anchors.centerIn: parent
        modal: true
        standardButtons: Dialog.Ok | Dialog.Cancel
        ColumnLayout {
            Label { text: "Nom :"; font.bold: true }
            TextField {
                id: nameInput
                Layout.fillWidth: true
                focus: true
                onAccepted: nameDialog.accept()
            }
        }
        onAccepted: {
            if (nameInput.text !== "" && root.tempFilePath !== "") {
                myPokemonModel.addPokemon(nameInput.text, root.tempFilePath)
                root.tempFilePath = ""
            }
        }
    }

    // --- 5. VUE PLEIN ÉCRAN ---
    Popup {
        id: fullScreenView
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        modal: true
        closePolicy: Popup.CloseOnEscape
        background: Rectangle { color: "#EE000000" }

        ColumnLayout {
            anchors.fill: parent
            spacing: 20

            // Bouton fermer en haut à droite
            Item {
                Layout.fillWidth: true; Layout.preferredHeight: 60
                Button {
                    text: "✖"
                    font.pixelSize: 20
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                    background: Rectangle { color: "transparent" }
                    contentItem: Text { text: parent.text; color: "white"; font.pixelSize: 24 }
                    onClicked: fullScreenView.close()
                }
            }

            // Image
            Image {
                Layout.fillWidth: true
                Layout.fillHeight: true
                source: root.currentPokemon.imgUrl || ""
                fillMode: Image.PreserveAspectFit
                mipmap: true
            }

            // Zone infos et boutons
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 180
                color: "#1A1A1A"
                radius: 20

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 15
                    width: parent.width * 0.9

                    Text {
                        text: root.currentPokemon.name || "Inconnu"
                        color: "white"
                        font.pixelSize: 24
                        font.bold: true
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        text: root.currentPokemon.isMine ? "👑 Propriétaire (Vous)" : "👤 Image d'un pair"
                        color: root.currentPokemon.isMine ? "#FFD700" : "#AAAAAA"
                        font.pixelSize: 12
                        Layout.alignment: Qt.AlignHCenter
                    }

                    // BOUTONS D'ACTION
                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 20

                        Button {
                            text: "📥 Télécharger"
                            background: Rectangle { color: "#007AFF"; radius: 8 }
                            contentItem: Text {
                                text: parent.text; color: "white"; font.bold: true
                                horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                                topPadding: 10; bottomPadding: 10; leftPadding: 20; rightPadding: 20
                            }
                            onClicked: saveFileDialog.open()
                        }

                        Button {
                            text: "🗑️ Supprimer"
                            background: Rectangle { color: parent.down ? "#800000" : "#CC0000"; radius: 8 }
                            contentItem: Text {
                                text: parent.text; color: "white"; font.bold: true
                                horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                                topPadding: 10; bottomPadding: 10; leftPadding: 20; rightPadding: 20
                            }
                            onClicked: deleteConfirmDialog.open()
                        }
                    }
                }
            }
        }
    }

    // --- GRID VIEW PRINCIPALE ---
    Timer {
        // Vérification de sécurité : si refreshFrequency n'existe pas, on met 2000ms
        interval: (myPokemonModel && myPokemonModel.refreshFrequency) ? myPokemonModel.refreshFrequency() : 2000
        running: true; repeat: true
        onTriggered: myPokemonModel.refreshPokemons()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 15

        // Header
        RowLayout {
            Layout.fillWidth: true; Layout.margins: 15; Layout.topMargin: 20
            Rectangle {
                width: 120; height: 40
                color: "#F0F2F5"; radius: 8
                Row {
                    anchors.centerIn: parent; spacing: 5
                    Text { text: "Tous (" + (myPokemonModel ? myPokemonModel.rowCount() : 0) + ")"; font.pixelSize: 14 }
                }
            }
            Item { Layout.fillWidth: true }
            Button {
                text: "+  Ajouter"
                background: Rectangle { color: "#050510"; radius: 8 }
                contentItem: Text {
                    text: parent.text; color: "white"; font.bold: true
                    horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
                }
                onClicked: addFileDialog.open()
            }
        }

        GridView {
            id: grid
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.leftMargin: 15; Layout.rightMargin: 15
            clip: true
            cellWidth: width / 2; cellHeight: 280

            model: myPokemonModel

            delegate: Item {
                width: grid.cellWidth
                height: grid.cellHeight

                PokemonDelegate {
                    anchors.fill: parent; anchors.margins: 5
                    name: model.name
                    pNumber: model.pNumber
                    type: model.type
                    size: model.size
                    imgUrl: model.imgUrl
                }

                TapHandler {
                    onTapped: {
                        root.currentIndex = index
                        root.currentPokemon = {
                            "name": model.name,
                            "imgUrl": model.imgUrl,
                            "size": model.size,
                            "type": model.type,
                            "pNumber": model.pNumber,
                            "isMine": model.isMine
                        }
                        fullScreenView.open()
                    }
                }
            }
        }
    }
}
