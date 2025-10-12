// Generated from Lottie file lottie/bin.json
import QtQuick
import QtQuick.VectorImage
import QtQuick.VectorImage.Helpers
import QtQuick.Shapes
import Qt.labs.lottieqt.VectorImageHelpers
import Tire 1.0

Item {
    readonly property var _fonts: Appearence.fonts
    readonly property var _gaps: Appearence.gaps
    readonly property var _radius: Appearence.radius
    readonly property var _color: Appearence.colors

    implicitWidth: 192
    implicitHeight: 192
    component AnimationsInfo : QtObject
    {
        property bool paused: false
        property int loops: 1
        signal restart()
    }
    property AnimationsInfo animations : AnimationsInfo {}
    transform: [
        Scale { xScale: width / 192; yScale: height / 192 }
    ]
    objectName: "_q_animation"
    id: _qt_node0
    LayerItem {
        objectName: "Layer 10 Outlines"
        id: _qt_layer0
        transform: TransformGroup {
            id: _qt_layer0_transform_base_group
            Matrix4x4 { id: _qt_layer0_transform_base; matrix: PlanarTransform.fromAffineMatrix(1, 0, 0, 1, 39.168, 44.334)}
        }
        Shape {
            id: _qt_node1
            transform: TransformGroup {
                id: _qt_node1_transform_base_group
                Matrix4x4 { id: _qt_node1_transform_base; matrix: PlanarTransform.fromAffineMatrix(1, 0, 0, 1, 56.832, 61.999)}
            }
            ShapePath {
                id: _qt_shapePath_0
                strokeColor: _color.main_contrast
                strokeWidth: 10.333
                capStyle: ShapePath.RoundCap
                joinStyle: ShapePath.RoundJoin
                miterLimit: 0
                fillColor: "#00000000"
                fillRule: ShapePath.WindingFill
                PathSvg { path: "M 20.667 36.167 C 20.667 36.167 -20.666 36.167 -20.666 36.167 C -26.374 36.167 -31 31.54 -31 25.833 C -31 25.833 -31 -36.167 -31 -36.167 C -31 -36.167 31 -36.167 31 -36.167 C 31 -36.167 31 25.833 31 25.833 C 31 31.54 26.374 36.167 20.667 36.167 " }
            }
        }
    }
    LayerItem {
        objectName: "comp_0"
        id: _qt_layer1
        transform: TransformGroup {
            id: _qt_layer1_transform_base_group
            TransformGroup {
                id: _qt_layer1_transform_group_0
                Translate { id: _qt_layer1_transform_0_3 }
                Scale { id: _qt_layer1_transform_0_2}
                Rotation { id: _qt_layer1_transform_0_1; origin.x: _qt_layer1.width / 2.0; origin.y: _qt_layer1.height / 2.0 }
                Translate { id: _qt_layer1_transform_0_0 }
            }
        }
        Connections { target: _qt_node0.animations; function onRestart() {_qt_layer1_transform_animation.restart() } }
        ParallelAnimation {
            id:_qt_layer1_transform_animation
            loops: _qt_node0.animations.loops
            paused: _qt_node0.animations.paused
            running: true
            onLoopsChanged: { if (running) { restart() } }
            SequentialAnimation {
                loops: 1
                ParallelAnimation {
                    SequentialAnimation {
                        ParallelAnimation {
                            ScriptAction {
                                script:_qt_layer1_transform_0_0.x = 203
                            }
                            ScriptAction {
                                script:_qt_layer1_transform_0_0.y = 87
                            }
                        }
                        PauseAnimation { duration: 1031 }
                    }
                    SequentialAnimation {
                        ParallelAnimation {
                            ScriptAction {
                                script:_qt_layer1_transform_0_1.origin = Qt.vector3d(0, 0, 0)
                            }
                            ScriptAction {
                                script:_qt_layer1_transform_0_1.angle = 0
                            }
                        }
                        PauseAnimation { duration: 63 }
                        ParallelAnimation {
                            PropertyAnimation {
                                duration: 375
                                target: _qt_layer1_transform_0_1
                                property: "origin"
                                to: Qt.vector3d(0, 0, 0)
                                easing.type: Easing.BezierSpline
                                easing.bezierCurve: [ 0.333, 0, 0.667, 1, 1, 1 ]
                            }
                            PropertyAnimation {
                                duration: 375
                                target: _qt_layer1_transform_0_1
                                property: "angle"
                                to: 13
                                easing.type: Easing.BezierSpline
                                easing.bezierCurve: [ 0.333, 0, 0.667, 1, 1, 1 ]
                            }
                        }
                        ParallelAnimation {
                            PropertyAnimation {
                                duration: 312
                                target: _qt_layer1_transform_0_1
                                property: "origin"
                                to: Qt.vector3d(0, 0, 0)
                                easing.type: Easing.BezierSpline
                                easing.bezierCurve: [ 0.333, 0, 0.667, 1, 1, 1 ]
                            }
                            PropertyAnimation {
                                duration: 312
                                target: _qt_layer1_transform_0_1
                                property: "angle"
                                to: 0
                                easing.type: Easing.BezierSpline
                                easing.bezierCurve: [ 0.333, 0, 0.667, 1, 1, 1 ]
                            }
                        }
                        PauseAnimation { duration: 281 }
                    }
                    SequentialAnimation {
                        ParallelAnimation {
                            ScriptAction {
                                script:_qt_layer1_transform_0_2.xScale = 1
                            }
                            ScriptAction {
                                script:_qt_layer1_transform_0_2.yScale = 1
                            }
                        }
                        PauseAnimation { duration: 1031 }
                    }
                    SequentialAnimation {
                        ParallelAnimation {
                            ScriptAction {
                                script:_qt_layer1_transform_0_3.x = -203
                            }
                            ScriptAction {
                                script:_qt_layer1_transform_0_3.y = -87
                            }
                        }
                        PauseAnimation { duration: 1031 }
                    }
                }
                ScriptAction {
                    script: {
                        _qt_layer1_transform_base_group.deactivateOverride(_qt_layer1_transform_group_0)
                    }
                }
            }
        }
        LayerItem {
            objectName: "Layer 9 Outlines"
            id: _qt_layer2
            transform: TransformGroup {
                id: _qt_layer2_transform_base_group
                Matrix4x4 { id: _qt_layer2_transform_base; matrix: PlanarTransform.fromAffineMatrix(1, 0, 0, 1, 54.669, 23.667)}
            }
            Shape {
                id: _qt_node2
                transform: TransformGroup {
                    id: _qt_node2_transform_base_group
                    Matrix4x4 { id: _qt_node2_transform_base; matrix: PlanarTransform.fromAffineMatrix(1, 0, 0, 1, 41.332, 36.166)}
                }
                ShapePath {
                    id: _qt_shapePath_1
                    strokeColor: _color.main_contrast
                    strokeWidth: 10.333
                    capStyle: ShapePath.RoundCap
                    joinStyle: ShapePath.RoundJoin
                    miterLimit: 0
                    fillColor: "#00000000"
                    fillRule: ShapePath.WindingFill
                    PathSvg { path: "M -5.167 -10.333 C -5.167 -10.333 5.166 -10.333 5.166 -10.333 C 10.874 -10.333 15.5 -5.707 15.5 0 C 15.5 0 15.5 10.333 15.5 10.333 C 15.5 10.333 -15.5 10.333 -15.5 10.333 C -15.5 10.333 -15.5 0 -15.5 0 C -15.5 -5.707 -10.874 -10.333 -5.167 -10.333 " }
                }
            }
        }
        LayerItem {
            objectName: "Layer 8 Outlines"
            id: _qt_layer3
            transform: TransformGroup {
                id: _qt_layer3_transform_base_group
                Matrix4x4 { id: _qt_layer3_transform_base; matrix: PlanarTransform.fromAffineMatrix(1, 0, 0, 1, 49.5, 65.001)}
            }
            Shape {
                id: _qt_node3
                ShapePath {
                    id: _qt_shapePath_2
                    strokeColor: _color.main_contrast
                    strokeWidth: 10.333
                    capStyle: ShapePath.RoundCap
                    joinStyle: ShapePath.RoundJoin
                    miterLimit: 0
                    fillColor: "#00000000"
                    fillRule: ShapePath.WindingFill
                    PathSvg { path: "M 5.166 5.166 C 5.166 5.166 87.833 5.166 87.833 5.166 " }
                }
            }
        }
    }
    LayerItem {
        objectName: "Layer 7 Outlines"
        id: _qt_layer4
        transform: TransformGroup {
            id: _qt_layer4_transform_base_group
            Matrix4x4 { id: _qt_layer4_transform_base; matrix: PlanarTransform.fromAffineMatrix(1, 0, 0, 1, 101.167, 85.667)}
        }
        Connections { target: _qt_node0.animations; function onRestart() {_qt_layer4_visible_animation.restart() } }
        ParallelAnimation {
            id: _qt_layer4_visible_animation
            loops: _qt_node0.animations.loops
            paused: _qt_node0.animations.paused
            running: true
            onLoopsChanged: { if (running) { restart() } }
            SequentialAnimation {
                SequentialAnimation {
                    loops: 1
                    ScriptAction {
                        script:_qt_layer4.visible = false
                    }
                    PauseAnimation { duration: 250 }
                    ScriptAction {
                        script:_qt_layer4.visible = true
                    }
                    PauseAnimation { duration: 781 }
                }
            }
        }
        Shape {
            id: _qt_node4
            ShapePath {
                id: _qt_shapePath_3
                strokeColor: _color.main_contrast
                strokeWidth: 10.333
                capStyle: ShapePath.RoundCap
                joinStyle: ShapePath.RoundJoin
                miterLimit: 0
                fillColor: "#00000000"
                trim.start: 0
                trim.end: 0
                trim.offset: 0
                fillRule: ShapePath.WindingFill
                PathSvg { path: "M 5.167 5.167 C 5.167 5.167 5.167 36.167 5.167 36.167 " }
            }
            Connections { target: _qt_node0.animations; function onRestart() {_qt_shapePath_3_trim_end_animation.restart() } }
            ParallelAnimation {
                id: _qt_shapePath_3_trim_end_animation
                loops: _qt_node0.animations.loops
                paused: _qt_node0.animations.paused
                running: true
                onLoopsChanged: { if (running) { restart() } }
                SequentialAnimation {
                    SequentialAnimation {
                        loops: 1
                        ScriptAction {
                            script:_qt_shapePath_3.trim.end = 0
                        }
                        PauseAnimation { duration: 250 }
                        PropertyAnimation {
                            duration: 156
                            target: _qt_shapePath_3.trim
                            property: "end"
                            to: 1
                            easing.type: Easing.BezierSpline
                            easing.bezierCurve: [ 0.333, 0, 0.667, 1, 1, 1 ]
                        }
                        PauseAnimation { duration: 625 }
                    }
                }
            }
        }
    }
    LayerItem {
        objectName: "Layer 6 Outlines"
        id: _qt_layer5
        transform: TransformGroup {
            id: _qt_layer5_transform_base_group
            Matrix4x4 { id: _qt_layer5_transform_base; matrix: PlanarTransform.fromAffineMatrix(1, 0, 0, 1, 80.501, 85.667)}
        }
        Connections { target: _qt_node0.animations; function onRestart() {_qt_layer5_visible_animation.restart() } }
        ParallelAnimation {
            id: _qt_layer5_visible_animation
            loops: _qt_node0.animations.loops
            paused: _qt_node0.animations.paused
            running: true
            onLoopsChanged: { if (running) { restart() } }
            SequentialAnimation {
                SequentialAnimation {
                    loops: 1
                    ScriptAction {
                        script:_qt_layer5.visible = false
                    }
                    PauseAnimation { duration: 250 }
                    ScriptAction {
                        script:_qt_layer5.visible = true
                    }
                    PauseAnimation { duration: 781 }
                }
            }
        }
        Shape {
            id: _qt_node5
            ShapePath {
                id: _qt_shapePath_4
                strokeColor: _color.main_contrast
                strokeWidth: 10.333
                capStyle: ShapePath.RoundCap
                joinStyle: ShapePath.RoundJoin
                miterLimit: 0
                fillColor: "#00000000"
                trim.start: 0
                trim.end: 0
                trim.offset: 0
                fillRule: ShapePath.WindingFill
                PathSvg { path: "M 5.166 5.167 C 5.166 5.167 5.166 36.167 5.166 36.167 " }
            }
            Connections { target: _qt_node0.animations; function onRestart() {_qt_shapePath_4_trim_end_animation.restart() } }
            ParallelAnimation {
                id: _qt_shapePath_4_trim_end_animation
                loops: _qt_node0.animations.loops
                paused: _qt_node0.animations.paused
                running: true
                onLoopsChanged: { if (running) { restart() } }
                SequentialAnimation {
                    SequentialAnimation {
                        loops: 1
                        ScriptAction {
                            script:_qt_shapePath_4.trim.end = 0
                        }
                        PauseAnimation { duration: 344 }
                        PropertyAnimation {
                            duration: 156
                            target: _qt_shapePath_4.trim
                            property: "end"
                            to: 1
                            easing.type: Easing.BezierSpline
                            easing.bezierCurve: [ 0.333, 0, 0.667, 1, 1, 1 ]
                        }
                        PauseAnimation { duration: 531 }
                    }
                }
            }
        }
    }
}
