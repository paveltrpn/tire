import QtQuick
import QtQuick.Window

import Tire 1.0

import ".."

Window {
    id: fileDialogRoot

    title: "File"

    readonly property var _fonts: Appearence.fonts
    readonly property var _gaps: Appearence.gaps
    readonly property var _radius: Appearence.radius
    readonly property var _color: Appearence.colors

    visible: true

    width: 640
    height: 480

    x: 100
    y: 100

    flags: Qt.Window | Qt.WindowStaysOnTopHint | Qt.FramelessWindowHint

    color: _color.background

    // Custom Title Bar
    Rectangle {
        id: titleBar
        height: 32
        color: "blue"
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        MouseArea {
            anchors.fill: parent

            property point lastMousePos: Qt.point(0, 0)
            property bool dragActive

            onPressed: {
                dragActive = true;
                lastMousePos = Qt.point(mouseX, mouseY);
            }

            onReleased: {
                dragActive = false;
            }

            onPositionChanged: {
                if (dragActive) {
                    const dx = mouseX - lastMousePos.x;
                    const dy = mouseY - lastMousePos.y;
                    fileDialogRoot.x += dx;
                    fileDialogRoot.y += dy;
                }
            }
        }

        Row {
            anchors.right: parent.right
            anchors.rightMargin: 5
            spacing: 5
            height: parent.height

            NpButton {
                text: "X"
                width: 32
                height: 32
                onClicked: fileDialogRoot.close()
            }
        }
    }
}
