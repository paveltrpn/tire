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

    height: settingsTabBar.height + settingsTabLayout.height

    radius: _radius.half

    color: _color.background_overlay_60

    TabBar {
        id: settingsTabBar

        height: 32
        width: parent.width

        NpTabButton {
            text: qsTr("Grid")

            anchors.top: parent.top
            height: parent.height

            topLeftRadius: _radius.half

            font: _fonts.text_body_accent
        }

        NpTabButton {
            text: qsTr("Test box")

            anchors.top: parent.top
            height: parent.height

            topRightRadius: _radius.half

            font: _fonts.text_body_accent
        }
    }

    StackLayout {
        id: settingsTabLayout

        anchors {
            top: settingsTabBar.bottom
            left: parent.left
            right: parent.right
        }

        height: currentIndex !== -1 ? children[settingsTabBar.currentIndex].height : 0
        currentIndex: settingsTabBar.currentIndex

        GridTab {
            id: gridTab
            Layout.fillWidth: true
        }

        TestBoxTab {
            id: testBoxTab
            Layout.fillWidth: true
        }
    }
}
