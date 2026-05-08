import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Tire 1.0

Rectangle {
    id: gridComponent

    readonly property var _fonts: Appearence.fonts
    readonly property var _gaps: Appearence.gaps
    readonly property var _radius: Appearence.radius
    readonly property var _color: Appearence.colors

    bottomLeftRadius: _radius.half
    bottomRightRadius: _radius.half

    color: _color.background_overlay_60

    ColumnLayout {
        id: gridSettingsLayout

        spacing: 2

        anchors {
            left: parent.left
            right: parent.right
        }

        Item {
            id: spacer1
            Layout.preferredWidth: parent.width
        }

        Rectangle {
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16
            color: "red"
        }

        Rectangle {
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16
            color: "red"
        }

        Rectangle {
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16
            color: "red"
        }

        Rectangle {
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16
            color: "red"
        }
    }
}
