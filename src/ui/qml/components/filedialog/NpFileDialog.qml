import QtQuick
import QtQuick.Window
import QtQuick.Layouts

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

    x: 300
    y: 300

    flags: Qt.Window | Qt.WindowStaysOnTopHint | Qt.FramelessWindowHint

    color: _color.background

    Rectangle {
        id: titleBar
        height: 32
        color: _color.background_overlay_40

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        MouseArea {
            id: headerArea
            anchors.fill: parent

            property point lastMousePos: Qt.point(0, 0)

            property bool dragActive
            propagateComposedEvents: true

            onPressed: {
                fileDialogRoot.startSystemMove();
            }
        }

        RowLayout {
            anchors {
                right: parent.right
                top: parent.top
                bottom: parent.bottom
            }

            spacing: _gaps.half

            NpButton {
                text: "X"

                Layout.preferredWidth: 24
                Layout.preferredHeight: 24
                Layout.alignment:  Qt.AlignVCenter | Qt.AlignLeft

                onClicked: fileDialogRoot.close()
            }

            Item {
                id: spacer1
            }
        }
    }
}
