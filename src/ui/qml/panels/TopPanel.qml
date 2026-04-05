// qmllint disable unqualified
// qmllint disable Quick.property-changes-parsed

import QtQuick
import QtQuick.Layouts

import Tire 1.0

import "../components"

Rectangle {
    id: rightPanelMainComponent

    readonly property var _fonts: Appearence.fonts
    readonly property var _gaps: Appearence.gaps
    readonly property var _radius: Appearence.radius
    readonly property var _color: Appearence.colors

    color: _color.background

    Item {
        id: mainTopPanel

        anchors {
            fill: parent
        }

        RowLayout {
            id: topPanelButtonsLayout

            spacing: _gaps.quarter

            anchors {
                fill: parent
            }

            NpButton {
                id: gizmoMoveModeBtn
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                icon.source: "image://TiredImageProvider/gear.svg"
                onClicked: {}
            }

            NpButton {
                id: gizmoRotateModeBtn
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                icon.source: "image://TiredImageProvider/gear.svg"
                onClicked: {}
            }

            NpButton {
                id: gizmoScaleModeBtn
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                icon.source: "image://TiredImageProvider/gear.svg"
                onClicked: {}
            }

            Item {
                id: buttonsSpacer
                Layout.fillWidth: true
            }

            Text {
                id: sclLabel

                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

                text: {
                    const pos = Tired.inputHandler.mousePos;
                    return `X: ${pos.x} Y: ${pos.y}`;
                }

                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter

                padding: 8

                color: _color.additional_contrast_60
                font: _fonts.label
            }
        }
    }
}
