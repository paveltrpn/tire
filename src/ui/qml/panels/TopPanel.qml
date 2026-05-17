// qmllint disable unqualified
// qmllint disable Quick.property-changes-parsed

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Tire 1.0

import "../components"
import "../components/filedialog"

Rectangle {
    id: topPanelMainComponent

    readonly property var _fonts: Appearence.fonts
    readonly property var _gaps: Appearence.gaps
    readonly property var _radius: Appearence.radius
    readonly property var _color: Appearence.colors

    color: _color.background

    topLeftRadius:  _radius.half
    topRightRadius: _radius.half

    MouseArea {
        id: headerArea
        anchors.fill: parent
        onPressed: {
            MainWindow.moveWindow();
        }
    }

    NpFileDialog {
        id: fileDialog
    }

    Item {
        id: menuBarWrapper

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        height: 32

        RowLayout {
            id: menuBarButtonsLayout

            spacing: 0

            anchors {
                left: parent.left
                leftMargin: _gaps.full
                top: parent.top
                bottom: parent.bottom
                right: mainWindowDecorationWrapper.left
            }

            NpMenuBarButton {
                id: fileMenuButton

                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                text: "File"
                font: _fonts.label_accent

                onClicked: {
                    fileMenu.open();
                }

                Popup {
                    id: fileMenu

                    y: parent.height

                    width: 56
                    height: 48 + 48 + 48

                    padding: 0

                    popupType: Popup.Native

                    contentItem: Rectangle {
                        color: "gray"
                        Column {
                            Rectangle {
                                width: 56
                                height: 24
                                color: "red"
                            }
                            Rectangle {
                                width: 56
                                height: 24
                                color: "blue"
                            }
                            Rectangle {
                                width: 56
                                height: 24
                                color: "red"
                            }
                            Rectangle {
                                width: 56
                                height: 24
                                color: "blue"
                            }
                            Rectangle {
                                width: 56
                                height: 24
                                color: "red"
                            }
                            Rectangle {
                                width: 56
                                height: 24
                                color: "blue"
                            }
                        }
                    }
                }
            }

            NpMenuBarButton {
                id: helpMenuButton

                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                text: "Help"
                font: _fonts.label_accent

                onClicked: {
                    helpMenu.open();
                }

                Popup {
                    id: helpMenu

                    y: parent.height

                    width: 56
                    height: 48 + 48 + 48

                    padding: 0

                    popupType: Popup.Window

                    contentItem: Rectangle {
                        color: "gray"
                        Column {
                            Rectangle {
                                width: 56
                                height: 24
                                color: "red"
                            }
                            Rectangle {
                                width: 56
                                height: 24
                                color: "blue"
                            }
                            Rectangle {
                                width: 56
                                height: 24
                                color: "red"
                            }
                            Rectangle {
                                width: 56
                                height: 24
                                color: "blue"
                            }
                            Rectangle {
                                width: 56
                                height: 24
                                color: "red"
                            }
                            Rectangle {
                                width: 56
                                height: 24
                                color: "blue"
                            }
                        }
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Item {
            id: mainWindowDecorationWrapper

            anchors {
                right: parent.right
                rightMargin: _gaps.full
                top: parent.top
                bottom: parent.bottom
            }

            width: 128

            NpButton {
                id: maximizeWindowButton

                anchors {
                    right: foldWindowButton.left
                    rightMargin: 4
                    top: parent.top
                }

                width: 24
                height: 24

                icon.source: "image://TiredImageProvider/arrow-up-right-from-square.svg"
                onClicked: {
                    Qt.quit();
                }
            }

            NpButton {
                id: foldWindowButton

                anchors {
                    right: closeAppButton.left
                    rightMargin: 4
                    top: parent.top
                }

                width: 24
                height: 24

                icon.source: "image://TiredImageProvider/square-minus.svg"
                onClicked: {
                    Qt.quit();
                }
            }

            NpButton {
                id: closeAppButton

                anchors {
                    right: parent.right
                    top: parent.top
                }

                width: 24
                height: 24

                icon.source: "image://TiredImageProvider/xmark-large.svg"
                onClicked: {
                    Qt.quit();
                }
            }
        }
    }

    Item {
        id: mainTopPanel

        anchors {
            left: parent.left
            right: parent.right
            top: menuBarWrapper.bottom
            topMargin: _gaps.half
        }

        height: 32

        RowLayout {
            id: topPanelButtonsLayout

            spacing: _gaps.half

            anchors {
                fill: parent
            }

            Item {
                id: spacerLeft
                Layout.preferredWidth: _gaps.half
            }

            NpButton {
                id: gizmoMoveModeBtn
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                icon.source: "image://TiredImageProvider/arrow-all-330.svg"
                onClicked: {}
            }

            NpButton {
                id: gizmoRotateModeBtn
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                icon.source: "image://TiredImageProvider/refresh-ccw-alt-4.svg"
                onClicked: {}
            }

            NpButton {
                id: gizmoScaleModeBtn
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                icon.source: "image://TiredImageProvider/arrow-corner-278.svg"
                onClicked: {}
            }

            Item {
                id: spacer2
                Layout.preferredWidth: _gaps.full
            }

            Item {
                id: gizmoCoordinateSystemModeBtnsWrapper

                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft

                Layout.preferredWidth: gizmoLocalModeBtn.width + gizmoGlobalModeBtn.width + _gaps.half
                // implicitWidth: gizmoLocalModeBtn.width + gizmoGlobalModeBtn.width

                NpButton {
                    id: gizmoLocalModeBtn

                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                    }

                    icon.source: "image://TiredImageProvider/cube-alt.svg"
                    onClicked: {}
                }

                NpButton {
                    id: gizmoGlobalModeBtn

                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                    }

                    Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                    icon.source: "image://TiredImageProvider/cube_01.svg"
                    onClicked: {
                        fileDialog.show();
                    }
                }
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

            Item {
                id: spacerRight
                Layout.preferredWidth: _gaps.half
            }
        }
    }
}
