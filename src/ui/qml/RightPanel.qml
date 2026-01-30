import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import Qt5Compat.GraphicalEffects
import Qt.labs.lottieqt 1.0

import Tire 1.0
import "components"

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
            id: mainLeftPanel

            Behavior on height {
                NumberAnimation {
                    duration: 200
                }
            }

            anchors {
                right: parent.right
                top: parent.top
                topMargin: rightPanelMainComponent._gaps.half
            }

            width: 40
            height: parent.height - 16

            color: rightPanelMainComponent._color.background_additional_40

            topRightRadius: 0
            bottomRightRadius: 0
            topLeftRadius: rightPanelMainComponent._radius.half
            bottomLeftRadius: rightPanelMainComponent._radius.half

            NpButton {
                id: testPanelButtom
                anchors {
                    top: parent.top
                    topMargin: rightPanelMainComponent._gaps.half
                    horizontalCenter: parent.horizontalCenter
                }

                icon.source: "icons/exit_up.svg"
                onClicked: {
                }
            }
        }
    }
}
