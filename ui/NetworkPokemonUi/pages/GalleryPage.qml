import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: root

    // Timer
    Timer {
        interval: myPokemonModel.refreshFrequency()
        running: true
        repeat: true
        onTriggered: myPokemonModel.refreshPokemons()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 15

        // Barre supérieure
        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 15
            Layout.topMargin: 20

            Rectangle {
                width: 120; height: 40
                color: "#F0F2F5"; radius: 8
                Row {
                    anchors.centerIn: parent; spacing: 5
                    Text { text: "Tous (" + myPokemonModel.rowCount() + ") ▼"; font.pixelSize: 14 }
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
                onClicked: myPokemonModel.refreshPokemons()
            }
        }

        GridView {
            id: grid
            Layout.fillWidth: true; Layout.fillHeight: true
            Layout.leftMargin: 15; Layout.rightMargin: 15
            clip: true
            cellWidth: width / 2; cellHeight: 280

            model: myPokemonModel

            delegate: PokemonDelegate {
                name: model.name
                pNumber: model.pNumber
                type: model.type
                size: model.size
                imgUrl: model.imgUrl
            }
        }
    }
}
