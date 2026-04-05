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

    Rectangle {
        id: mainLeftPanel

        anchors {
            left: parent.left
            top: parent.top
            topMargin: _gaps.half
            bottom: parent.bottom
            bottomMargin: _gaps.half
        }

        width: 40

        color: leftPanelMainComponent._color.background_overlay_60

        topRightRadius: _radius.half
        bottomRightRadius: _radius.half
        topLeftRadius: 0
        bottomLeftRadius: 0

        ColumnLayout {
            id: mainButtonsLayout

            spacing: _gaps.half

            anchors {
                fill: parent
            }

            Item {
                id: spacerTop
                Layout.preferredHeight: _gaps.half
            }

            NpButton {
                id: addBoxBtn
                Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
                icon.source: "image://TiredImageProvider/cube_01.svg"
                onClicked: {
                    if (addBoxBtn.checked) {
                        addBoxBtn.checked = false;
                    } else {
                        leftPanelMainComponent.resetAllButtons();
                        addBoxBtn.checked = !addBoxBtn.checked;
                    }
                }
            }

            Item {
                id: buttonsSpacer
                Layout.fillHeight: true
            }

            NpButton {
                id: settingsBtn
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                icon.source: "image://TiredImageProvider/gear.svg"
                onClicked: {}
            }

            NpButton {
                id: showInfoBtn
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                icon.source: "image://TiredImageProvider/info.svg"
                onClicked: {
                    Tired.grid.setGridSize(1.5);

                    if (showInfoBtn.checked) {
                        showInfoBtn.checked = false;
                    } else {
                        leftPanelMainComponent.resetAllButtons();
                        showInfoBtn.checked = !showInfoBtn.checked;
                    }
                }
            }

            NpButton {
                id: closeBtn
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                icon.source: "image://TiredImageProvider/power.svg"
                onClicked: {
                    Qt.quit();
                }
            }

            Item {
                id: spacerBottom
                Layout.preferredHeight: _gaps.half
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

        visible: showInfoBtn.checked
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

        visible: addBoxBtn.checked
    }

    function resetAllButtons() {
        addBoxBtn.checked = false;
        showInfoBtn.checked = false;
    }
}
