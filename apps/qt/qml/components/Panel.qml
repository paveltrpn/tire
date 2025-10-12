
import QtQuick 2.15
import QtQuick.Controls 2.15
import NetPet 1.0

Rectangle {
    id: control
    radius: 8
    color: Theme.colors["background"]

    Rectangle {
        id: background
        radius: 8
        color: Theme.colors["background_additional_20"]
        anchors {
            fill: parent
            topMargin: 8
            bottomMargin: 8
            leftMargin: 8
            rightMargin: 8
        }
    }
}
