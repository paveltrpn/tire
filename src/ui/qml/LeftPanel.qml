import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import Qt5Compat.GraphicalEffects
import Qt.labs.lottieqt 1.0

import Tire 1.0
import "components"
import "info"

Rectangle {
    id: leftPanelMainComponent

    readonly property var _fonts: Appearence.fonts
    readonly property var _gaps: Appearence.gaps
    readonly property var _radius: Appearence.radius
    readonly property var _color: Appearence.colors

    color: _color.background

    Item {
        id: leftPanelComponentBackground

        width: leftPanelMainComponent.width
        height: leftPanelMainComponent.height

        property bool isUiHidden: hidePanelButtom.checked

        Rectangle {
            id: mainLeftPanel

            Behavior on height {
                NumberAnimation {
                    duration: 200
                }
            }

            anchors {
                left: parent.left
                top: parent.top
                topMargin: leftPanelMainComponent._gaps.half
            }

            width: 40

            color: leftPanelMainComponent._color.background_additional_40

            topRightRadius: leftPanelMainComponent._radius.half
            bottomRightRadius: leftPanelMainComponent._radius.half
            topLeftRadius: 0
            bottomLeftRadius: 0

            NpButton {
                id: hidePanelButtom
                anchors {
                    top: parent.top
                    topMargin: leftPanelMainComponent._gaps.half
                    horizontalCenter: parent.horizontalCenter
                }

                icon.source: "icons/exit_up.svg"
                onClicked: {
                    hidePanelButtom.checked = !hidePanelButtom.checked
                }
            }

            NpButton {
                id: closeButton
                anchors {
                    bottom: parent.bottom
                    bottomMargin: leftPanelMainComponent._gaps.half
                    horizontalCenter: parent.horizontalCenter
                }
                icon.source: "icons/power.svg"
                onClicked: {
                    Qt.quit()
                }
            }

            states: [
                State {
                    when: leftPanelComponentBackground.isUiHidden
                    PropertyChanges {
                        target: mainLeftPanel
                        height: hidePanelButtom.height + closeButton.height + 8 * 3
                    }
                },
                State {
                    when: !leftPanelComponentBackground.isUiHidden
                    PropertyChanges {
                        target: mainLeftPanel
                        height: parent.height - 16
                    }
                }
            ]

            Rectangle {
                id: mainButtonsHolder

                anchors {
                    left: parent.left
                    right: parent.right
                    top: hidePanelButtom.bottom
                    topMargin: leftPanelMainComponent._gaps.half
                    bottom: closeButton.top
                    bottomMargin: leftPanelMainComponent._gaps.half
                }

                color: "transparent"

                visible: !leftPanelComponentBackground.isUiHidden

                NpButton {
                    id: showInfo
                    anchors {
                        bottom: parent.bottom
                        horizontalCenter: parent.horizontalCenter
                    }
                    icon.source: "icons/info.svg"
                    onClicked: {
                        showInfo.checked = !showInfo.checked
                    }
                }
            }
        }

        Rectangle {
            id: infoWrapper

            color: "transparent"
            anchors {
                top: mainLeftPanel.top
                bottom: mainLeftPanel.bottom
                left: mainLeftPanel.right
                right: leftPanelComponentBackground.right
                rightMargin: leftPanelMainComponent._gaps.half
            }

            visible: showInfo.checked && !leftPanelComponentBackground.isUiHidden

            Services {
                id: servicesInfoWidget
                anchors {
                    top: parent.top
                    left: parent.left
                    leftMargin: leftPanelMainComponent._gaps.half
                    right: parent.right
                }
            }
        }


        /*
        DragAbleItem {
            id: buttonsPanel

            height: reloadModelsButton.height + 16
            width: 128 * 3

            x: 32
            y: 32

            radius: _radius.half
            color: _color.background
        }
        */
    }
}
