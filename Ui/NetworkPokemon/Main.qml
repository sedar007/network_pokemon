import QtQuick
import QtQuick.Controls

Window {
    visible: true
    visibility: Window.FullScreen    // 👈 plein écran
    title: "Pokemon Test"

    Column {
        anchors.centerIn: parent
        spacing: 15

        TextField {
            id: nameField
            placeholderText: "Enter name"
            width: 200
            color: black
        }

        TextField {
            id: ageField
            placeholderText: "Enter age"
            width: 200
            inputMethodHints: Qt.ImhDigitsOnly
            color: black
        }

        Button {
            text: "Send"
            onClicked: {
                if (ageField.text !== "" && !isNaN(ageField.text)) {
                    testObj.age = parseInt(ageField.text)
                }
                if (nameField.text !== "") {
                    testObj.name = nameField.text
                }
            }
        }

        // Affichage des valeurs venant du C++
        Text { text: "Name: " + testObj.name; font.pixelSize: 20 }
        Text { text: "Age: " + testObj.age; font.pixelSize: 20 }
    }
}
