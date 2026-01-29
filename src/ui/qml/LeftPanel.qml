import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import Qt5Compat.GraphicalEffects
import Qt.labs.lottieqt 1.0

import Tire 1.0
import "components"
import "info"

Rectangle {
    id: mainWindow

    readonly property var _fonts: Appearence.fonts
    readonly property var _gaps: Appearence.gaps
    readonly property var _radius: Appearence.radius
    readonly property var _color: Appearence.colors

    color: _color.background

    Item {
        id: mainUiComponentBackground

        width: mainWindow.width
        height: mainWindow.height

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
                leftMargin: 16
                top: parent.top
                topMargin: 16
            }

            width: 56

            color: _color.background_additional_40
            radius: 8

            NpButton {
                id: hidePanelButtom
                anchors {
                    top: parent.top
                    topMargin: 8
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
                    bottomMargin: 8
                    horizontalCenter: parent.horizontalCenter
                }
                icon.source: "icons/power.svg"
                onClicked: {
                    Qt.quit()
                }
            }

            states: [
                State {
                    when: mainUiComponentBackground.isUiHidden
                    PropertyChanges {
                        target: mainLeftPanel
                        height: hidePanelButtom.height + closeButton.height + 8 * 3
                    }
                },
                State {
                    when: !mainUiComponentBackground.isUiHidden
                    PropertyChanges {
                        target: mainLeftPanel
                        height: parent.height - 32
                    }
                }
            ]

            Rectangle {
                id: mainButtonsHolder

                anchors {
                    left: parent.left
                    right: parent.right
                    top: hidePanelButtom.bottom
                    topMargin: 8
                    bottom: closeButton.top
                    bottomMargin: 8
                }

                color: "transparent"

                visible: !mainUiComponentBackground.isUiHidden

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
                right: mainUiComponentBackground.right
                rightMargin: _gaps.full
            }

            visible: showInfo.checked
                     && !mainUiComponentBackground.isUiHidden

            Services {
                id: servicesInfoWidget
                anchors {
                    top: parent.top
                    left: parent.left
                    leftMargin: 16
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
