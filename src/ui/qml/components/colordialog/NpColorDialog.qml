import QtQuick
import QtQuick.Window

import Tire 1.0

Window {
    id: colorDialogRoot

    title: "Color"

    readonly property var _fonts: Appearence.fonts
    readonly property var _gaps: Appearence.gaps
    readonly property var _radius: Appearence.radius
    readonly property var _color: Appearence.colors

    visible: true

    width: 640
    height: 480

    x: 100
    y: 100

    flags: Qt.Window | Qt.WindowStaysOnTopHint//| Qt.FramelessWindowHint

    color: _color.background
}

