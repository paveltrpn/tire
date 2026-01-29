import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import Qt5Compat.GraphicalEffects
import Qt.labs.lottieqt 1.0

Item {
    id: mainWindow

    anchors.fill: parent

    Component.onCompleted: {
        console.log("=== qml loaded")
    }

    Rectangle {
        anchors {
            centerIn: parent
        }

        width: 128
        height: 128

        color: "red"
    }
}
