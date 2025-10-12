// Generated from Lottie file lottie/texting.json
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
        objectName: "Layer 5 Outlines"
        id: _qt_layer0
        transform: TransformGroup {
            id: _qt_layer0_transform_base_group
            Matrix4x4 { id: _qt_layer0_transform_base; matrix: PlanarTransform.fromAffineMatrix(1, 0, 0, 1, 64.999, 80.501)}
        }
        Shape {
            id: _qt_node1
            ShapePath {
                id: _qt_shapePath_0
                strokeColor: _color.main_contrast
                strokeWidth: 10.333
                capStyle: ShapePath.RoundCap
                joinStyle: ShapePath.RoundJoin
                miterLimit: 0
                fillColor: "#00000000"
                trim.start: 0
                trim.end: 1
                trim.offset: 0
                fillRule: ShapePath.WindingFill
                PathSvg { path: "M 56.833 5.166 C 56.833 5.166 5.166 5.166 5.166 5.166 " }
            }
            Connections { target: _qt_node0.animations; function onRestart() {_qt_shapePath_0_trim_end_animation.restart() } }
            ParallelAnimation {
                id: _qt_shapePath_0_trim_end_animation
                loops: _qt_node0.animations.loops
                paused: _qt_node0.animations.paused
                running: true
                onLoopsChanged: { if (running) { restart() } }
                SequentialAnimation {
                    SequentialAnimation {
                        loops: 1
                        ScriptAction {
                            script:_qt_shapePath_0.trim.end = 1
                        }
                        PauseAnimation { duration: 344 }
                        PropertyAnimation {
                            duration: 250
                            target: _qt_shapePath_0.trim
                            property: "end"
                            to: 0
                            easing.type: Easing.BezierSpline
                            easing.bezierCurve: [ 0.333, 0, 0.667, 1, 1, 1 ]
                        }
                        PauseAnimation { duration: 437 }
                    }
                }
            }
        }
    }
    LayerItem {
        objectName: "Layer 4 Outlines"
        id: _qt_layer1
        transform: TransformGroup {
            id: _qt_layer1_transform_base_group
            Matrix4x4 { id: _qt_layer1_transform_base; matrix: PlanarTransform.fromAffineMatrix(1, 0, 0, 1, 65.001, 101.167)}
        }
        Shape {
            id: _qt_node2
            ShapePath {
                id: _qt_shapePath_1
                strokeColor: _color.main_contrast
                strokeWidth: 10.333
                capStyle: ShapePath.RoundCap
                joinStyle: ShapePath.RoundJoin
                miterLimit: 0
                fillColor: "#00000000"
                trim.start: 0
                trim.end: 1
                trim.offset: 0
                fillRule: ShapePath.WindingFill
                PathSvg { path: "M 36.166 5.167 C 36.166 5.167 5.166 5.167 5.166 5.167 " }
            }
            Connections { target: _qt_node0.animations; function onRestart() {_qt_shapePath_1_trim_end_animation.restart() } }
            ParallelAnimation {
                id: _qt_shapePath_1_trim_end_animation
                loops: _qt_node0.animations.loops
                paused: _qt_node0.animations.paused
                running: true
                onLoopsChanged: { if (running) { restart() } }
                SequentialAnimation {
                    SequentialAnimation {
                        loops: 1
                        ScriptAction {
                            script:_qt_shapePath_1.trim.end = 1
                        }
                        PauseAnimation { duration: 63 }
                        PropertyAnimation {
                            duration: 250
                            target: _qt_shapePath_1.trim
                            property: "end"
                            to: 0
                            easing.type: Easing.BezierSpline
                            easing.bezierCurve: [ 0.333, 0, 0.667, 1, 1, 1 ]
                        }
                        PauseAnimation { duration: 718 }
                    }
                }
            }
        }
    }
    LayerItem {
        objectName: "Layer 10 Outlines"
        id: _qt_layer2
        transform: TransformGroup {
            id: _qt_layer2_transform_base_group
            Matrix4x4 { id: _qt_layer2_transform_base; matrix: PlanarTransform.fromAffineMatrix(1, 0, 0, 1, 23.668, 23.668)}
        }
        Shape {
            id: _qt_node3
            transform: TransformGroup {
                id: _qt_node3_transform_base_group
                Matrix4x4 { id: _qt_node3_transform_base; matrix: PlanarTransform.fromAffineMatrix(1, 0, 0, 1, 72.332, 72.332)}
            }
            ShapePath {
                id: _qt_shapePath_2
                strokeColor: _color.main_contrast
                strokeWidth: 10.333
                capStyle: ShapePath.RoundCap
                joinStyle: ShapePath.RoundJoin
                miterLimit: 0
                fillColor: "#00000000"
                trim.start: 0
                trim.end: 1
                trim.offset: 0
                fillRule: ShapePath.WindingFill
                PathSvg { path: "M 0 -46.5 C -25.681 -46.5 -46.5 -25.682 -46.5 0 C -46.5 12.841 -41.295 24.466 -32.88 32.881 C -32.88 32.881 -46.5 46.5 -46.5 46.5 C -46.5 46.5 0 46.5 0 46.5 C 25.682 46.5 46.5 25.682 46.5 0 C 46.5 -25.682 25.682 -46.5 0 -46.5 " }
            }
        }
    }
}
