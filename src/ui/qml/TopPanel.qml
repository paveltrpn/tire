import QtQuick 2.15

import Tire 1.0

Rectangle {
    id: rightPanelMainComponent

    readonly property var _fonts: Appearence.fonts
    readonly property var _gaps: Appearence.gaps
    readonly property var _radius: Appearence.radius
    readonly property var _color: Appearence.colors

    color: _color.background

    Item {
        id: rightPanelComponentBackground

        width: rightPanelMainComponent.width
        height: rightPanelMainComponent.height

        Rectangle {
            id: mainPanel

            Behavior on height {
                NumberAnimation {
                    duration: 200
                }
            }

            anchors {
                right: parent.right
                rightMargin: _gaps.half
                verticalCenter: parent.verticalCenter
            }

            height: 24
            width: 160

            color: rightPanelMainComponent._color.background_overlay_60
            radius: rightPanelMainComponent._radius.half

            border {
                width: 2
                color: _color.main_contrast_20
            }

            Text {
                id: sclLabel
                anchors {
                    fill: parent
                }

                height: 24

                text: {
                     const pos = InputHandler.mousePos
                     return `X: ${pos.x} Y: ${pos.y}`
                }

                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter

                padding: 8

                color: _color.additional_contrast_60
                font: _fonts.label

            }
        }
    }
}
