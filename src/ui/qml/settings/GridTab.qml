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

    bottomLeftRadius:  _radius.half
    bottomRightRadius:  _radius.half

    color: "blue"//_color.background_overlay_60
}
