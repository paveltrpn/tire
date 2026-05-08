// qmllint disable unqualified

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Tire 1.0
import "../components"

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
            Layout.preferredHeight: 24

            onLoaded: {
                item.parameterlabel = "gridSize";
                item.parameterValue = Tired.grid.gridSize;
            }
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 24

            onLoaded: {
                item.parameterlabel = "lineThickness";
                item.parameterValue = Tired.grid.lineThickness;
            }
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 24

            onLoaded: {
                item.parameterlabel = "maxRange";
                item.parameterValue = Tired.grid.maxRange;
            }
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 24

            onLoaded: {
                item.parameterlabel = "zoomSensitivity";
                item.parameterValue = Tired.grid.zoomSensitivity;
            }
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 24

            onLoaded: {
                item.parameterlabel = "majorDivisor";
                item.parameterValue = Tired.grid.majorDivisor;
            }
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 24

            onLoaded: {
                item.parameterlabel = "gridScale";
                item.parameterValue = Tired.grid.gridScale;
            }
        }

        Loader {
            sourceComponent: gridParameter
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 24

            onLoaded: {
                item.parameterlabel = "gridZOffset";
                item.parameterValue = Tired.grid.gridZOffset;
            }
        }
    }

    Component {
        id: gridParameter

        Rectangle {
            id: gridParameterWrapper

            anchors.fill: parent
            color: _color.background_overlay_60

            property string parameterlabel
            property real parameterValue

            Text {
                id: paramLabel
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    leftMargin: 8
                }

                text: gridParameterWrapper.parameterlabel

                verticalAlignment: Text.AlignVCenter

                color: _color.main_contrast
                font: _fonts.label
            }

            TiredTextInput {
                id: paramValue

                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    right: parent.right
                    rightMargin: 8
                }

                width: 92

                text: gridParameterWrapper.parameterValue.toFixed(3)
            }
        }
    }
}
