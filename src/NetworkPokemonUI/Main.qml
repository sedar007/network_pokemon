import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "components" // Si tes composants sont dans un sous-dossier
import "pages"
import "Data"

ApplicationWindow {
    visible: true
    width: 375
    height: 812
    title: "Network Pokémon"
    color: "#F6F8FA"

    // On instancie la barre de navigation ici pour y accéder via son ID
    NavBar {
        id: navBar
    }

    header: AppHeader {
        currentIndex: navBar.currentIndex
    }

    footer: navBar

    StackLayout {
        anchors.fill: parent
        currentIndex: navBar.currentIndex

        // Page 1
        GalleryPage {}

        // Page 2 (Placeholder)
        PeersPage {}

        // Page 3 (Placeholder)
        Item { Text { text: "Page Recherche"; anchors.centerIn: parent } }

        // Page 4
        ProfilePage {}
    }
}
