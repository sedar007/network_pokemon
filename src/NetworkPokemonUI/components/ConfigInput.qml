import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    property string label
    property string placeholder
    property alias textValue: field.text // Alias pour récupérer le texte de l'extérieur

    Layout.fillWidth: true
    spacing: 8

    Text {
        text: label
        font.weight: Font.DemiBold
        font.pixelSize: 14
        color: "#333"
    }

    TextField {
        id: field
        text: placeholder
        placeholderText: placeholder
        Layout.fillWidth: true
        Layout.preferredHeight: 50
        background: Rectangle {
            color: "#F2F2F7"
            radius: 12
            border.color: field.activeFocus ? "black" : "transparent"
            border.width: 1
        }
        font.pixelSize: 16
        leftPadding: 16
    }
}
