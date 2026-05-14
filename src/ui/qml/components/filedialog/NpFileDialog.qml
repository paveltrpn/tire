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

    x: 400
    y: 400

    flags: Qt.Window | Qt.WindowStaysOnTopHint | Qt.FramelessWindowHint

    color: _color.background

    // Custom Title Bar
    Rectangle {
        id: titleBar
        height: 32
        color: "blue"

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        MouseArea {
            anchors.fill: parent

            property point lastMousePos: Qt.point(0, 0)

            property bool dragActive
            propagateComposedEvents: true

            onPressed: {
                dragActive = true;
                lastMousePos = Tired.inputHandler.mousePos;
            }

            onReleased: {
                dragActive = false;
            }

            onPositionChanged: {
                if (dragActive) {
                    const nowPos = Tired.inputHandler.mousePos

                    const dx = nowPos.x - lastMousePos.x;
                    const dy = nowPos.y - lastMousePos.y;
                    fileDialogRoot.x = nowPos.x;
                    fileDialogRoot.y = nowPos.y;
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
