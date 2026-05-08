import QtQuick
import QtQuick.Controls
import Tire 1.0

Rectangle {
    id: settingsComponent

    readonly property var _fonts: Appearence.fonts
    readonly property var _gaps: Appearence.gaps
    readonly property var _radius: Appearence.radius
    readonly property var _color: Appearence.colors

    height: 512

    z: parent.z

    radius: _radius.half
    color: _color.background_overlay_60
}
