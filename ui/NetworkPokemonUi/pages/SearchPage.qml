import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: root

    property string query: ""
    // Incrémenté périodiquement pour forcer le recalcul de "results" : myPokemonModel.get()
    // est un appel de fonction, pas une propriété observée, donc le binding ci-dessous ne
    // se réévaluerait pas tout seul quand la liste change côté C++.
    property int refreshTick: 0

    Timer {
        interval: (myPokemonModel && myPokemonModel.refreshFrequency) ? myPokemonModel.refreshFrequency() : 2000
        running: true; repeat: true
        onTriggered: root.refreshTick++
    }

    // myPokemonModel agrège déjà nos images et celles des pairs connus (gossip
    // GET_PICS) : la "recherche globale" est un filtre texte sur cette liste,
    // il n'existe pas (et n'a pas besoin d'exister) de requête réseau dédiée.
    property var results: {
        var list = []
        root.refreshTick // dépendance volontaire, voir commentaire ci-dessus
        if (!myPokemonModel) return list

        var needle = query.trim().toLowerCase()
        for (var i = 0; i < myPokemonModel.rowCount(); i++) {
            var item = myPokemonModel.get(i)
            if (needle === "" || item.name.toLowerCase().indexOf(needle) !== -1) {
                list.push(item)
            }
        }
        return list
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 15

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 15
            Layout.topMargin: 20

            TextField {
                id: searchInput
                Layout.fillWidth: true
                placeholderText: "Rechercher un Pokémon (nom)..."
                onTextChanged: root.query = text
                background: Rectangle { color: "#F0F2F5"; radius: 10 }
            }
        }

        Text {
            visible: root.results.length === 0
            text: root.query === "" ? "Tape un nom pour chercher parmi les images connues." : "Aucun résultat pour « " + root.query + " »."
            color: "#7F8C8D"
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 40
        }

        GridView {
            id: grid
            visible: root.results.length > 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: 15
            Layout.rightMargin: 15
            clip: true
            readonly property int columns: Math.max(2, Math.floor(width / 190))
            cellWidth: width / columns
            cellHeight: 280

            model: root.results

            delegate: Item {
                width: grid.cellWidth
                height: grid.cellHeight

                PokemonDelegate {
                    anchors.fill: parent
                    anchors.margins: 5
                    name: modelData.name
                    pNumber: modelData.pNumber
                    type: modelData.type
                    size: modelData.size
                    sizeUnit: modelData.sizeUnit
                    imgUrl: modelData.imgUrl
                }
            }
        }
    }
}
