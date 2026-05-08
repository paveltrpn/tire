// qmllint disable unqualified

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

    height: gridSettingsLayout.implicitHeight + _gaps.full

    ColumnLayout {
        id: gridSettingsLayout

        spacing: _gaps.half

        anchors {
            left: parent.left
            right: parent.right
        }

        Item {
            id: spacer1
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: _gaps.half
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16

            onLoaded: item.parameterlabel = "gridSize"
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16

            onLoaded: item.parameterlabel = "lineThickness"
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16

            onLoaded: item.parameterlabel = "maxRange"
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16

            onLoaded: item.parameterlabel = "zoomSensitivity"
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16

            onLoaded: item.parameterlabel = "majorDivisor"
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16

            onLoaded: item.parameterlabel = "gridScale"
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 16

            onLoaded: item.parameterlabel = "gridZOffset"
        }
    }

    Component {
        id: gridParameter
        Rectangle {
            id: gridParameterWrapper
            anchors.fill: parent
            color: _color.background_overlay_60

            property string parameterlabel

            Text {
                id: posLabel
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    leftMargin: 8
                }

                text: gridParameterWrapper.parameterlabel

                color: _color.main_contrast
                font: _fonts.label
            }
        }
    }
}
