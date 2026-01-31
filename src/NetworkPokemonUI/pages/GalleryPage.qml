import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components" // Pour accéder à PokemonDelegate
import "../Data"       // Pour accéder au Model

Item {
    ColumnLayout {
        anchors.fill: parent
        spacing: 15

        // Barre supérieure de la galerie (Filtre + Bouton Ajouter)
        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 15
            Layout.topMargin: 20

            Rectangle {
                width: 120; height: 40
                color: "#F0F2F5"; radius: 8
                Row {
                    anchors.centerIn: parent; spacing: 5
                    Text { text: "Tous (6) ▼"; font.pixelSize: 14 }
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
            }
        }

        GridView {
            id: grid
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.leftMargin: 15; Layout.rightMargin: 15
            clip: true
            cellWidth: width / 2; cellHeight: 280

            // On utilise le fichier séparé pour le modèle
            model: PokemonModel {}

            // On utilise le composant séparé pour l'affichage
            delegate: PokemonDelegate {
                // Mapping des données du modèle vers les propriétés du composant
                name: model.name
                pNumber: model.pNumber
                type: model.type
                size: model.size
                imgUrl: model.imgUrl
            }
        }
    }
}
