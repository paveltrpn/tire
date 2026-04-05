// qmllint disable unqualified
// qmllint disable Quick.property-changes-parsed

import QtQuick
import QtQuick.Layouts

import Tire 1.0

import "../components"
import "../info"
import "../add_forms"

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

            color: leftPanelMainComponent._color.background_overlay_60

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

                icon.source: "image://TiredImageProvider/exit_up.svg"
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
                icon.source: "image://TiredImageProvider/power.svg"
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
                    id: addBoxButtom
                    anchors {
                        top: parent.top
                        horizontalCenter: parent.horizontalCenter
                    }
                    icon.source: "image://TiredImageProvider/cube_01.svg"
                    onClicked: {
                        if (addBoxButtom.checked) {
                            addBoxButtom.checked = false
                        } else {
                            leftPanelMainComponent.resetAllButtons()
                            addBoxButtom.checked = !addBoxButtom.checked
                        }
                    }
                }

                NpButton {
                    id: showInfo
                    anchors {
                        bottom: parent.bottom
                        horizontalCenter: parent.horizontalCenter
                    }
                    icon.source: "image://TiredImageProvider/info.svg"
                    onClicked: {
                        Tired.grid.setGridSize(1.5)

                        if (showInfo.checked) {
                            showInfo.checked = false
                        } else {
                            leftPanelMainComponent.resetAllButtons()
                            showInfo.checked = !showInfo.checked
                        }
                    }
                }
            }
        }

        Services {
            id: servicesInfoWidget
            anchors {
                top: mainLeftPanel.top
                left: mainLeftPanel.right
                leftMargin: leftPanelMainComponent._gaps.half
                right: parent.right
                rightMargin: leftPanelMainComponent._gaps.half
            }

            visible: showInfo.checked
                     && !leftPanelComponentBackground.isUiHidden
        }

        AddBox {
            id: addBoxWidget
            anchors {
                top: mainLeftPanel.top
                left: mainLeftPanel.right
                leftMargin: leftPanelMainComponent._gaps.half
                right: parent.right
                rightMargin: leftPanelMainComponent._gaps.half
            }

            visible: addBoxButtom.checked
                     && !leftPanelComponentBackground.isUiHidden
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

    function resetAllButtons() {
        addBoxButtom.checked = false
        showInfo.checked = false
    }
}
