import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "components" // Si tes composants sont dans un sous-dossier
import "pages"

ApplicationWindow {
    id: window
    visible: true
    width: 420
    height: 840
    minimumWidth: 340
    minimumHeight: 480
    title: "Network Pokémon"
    color: "#F6F8FA"

    // Seuil de bascule entre navigation mobile (barre du bas, iPhone/fenêtre
    // étroite) et navigation desktop (barre latérale, macOS/fenêtre large).
    // window.currentIndex est la source de vérité unique : les deux barres de
    // navigation ne sont jamais instanciées en même temps (Loader), donc l'état
    // ne peut pas vivre dans l'une ou l'autre.
    readonly property bool isWide: width >= 720
    property int currentIndex: 0

    header: AppHeader {
        currentIndex: window.currentIndex
    }

    footer: Loader {
        active: !window.isWide
        height: item ? item.height : 0
        sourceComponent: NavBar {
            currentIndex: window.currentIndex
            onCurrentIndexChanged: window.currentIndex = currentIndex
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Loader {
            active: window.isWide
            Layout.fillHeight: true
            Layout.preferredWidth: item ? item.implicitWidth : 0
            sourceComponent: SideNavBar {
                currentIndex: window.currentIndex
                onCurrentIndexChanged: window.currentIndex = currentIndex
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: window.currentIndex

            // Page 1
            GalleryPage {}

            // Page 2
            PeersPage {}

            // Page 3
            SearchPage {}

            // Page 4
            ProfilePage {}
        }
    }
}
