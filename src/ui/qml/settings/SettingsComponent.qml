import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Tire 1.0

import "../components"

Rectangle {
    id: settingsComponent

    readonly property var _fonts: Appearence.fonts
    readonly property var _gaps: Appearence.gaps
    readonly property var _radius: Appearence.radius
    readonly property var _color: Appearence.colors

    height: 512

    radius: _radius.half
    color: _color.background_overlay_60

    TabBar {
        id: settingsTabBar

        height: 48
        width: parent.width

        NpTabButton {
            text: qsTr("Grid")

            anchors.top: parent.top
            height: 48
        }

        NpTabButton {
            text: qsTr("Test box")

            anchors.top: parent.top
            height: 48
        }
    }

    StackLayout {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: settingsTabBar.bottom
        }

        currentIndex: settingsTabBar.currentIndex

        TestBox {
            id: testBoxTab

            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Grid {
            id: gridTab

            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
