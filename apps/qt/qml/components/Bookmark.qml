// Generated from Lottie file lottie/bookmark.json
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
        objectName: "bookmark Outlines"
        id: _qt_layer0
        Shape {
            id: _qt_node1
            transform: TransformGroup {
                id: _qt_node1_transform_base_group
                Matrix4x4 { id: _qt_node1_transform_base; matrix: PlanarTransform.fromAffineMatrix(1, 0, 0, 1, 96, 96)}
            }
            ShapePath {
                id: _qt_shapePath_0
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
                PathSvg { path: "M -31 46.5 C -31 46.5 -30.948 46.448 -30.948 46.448 C -30.948 46.448 0 20.512 0 20.512 C 0 20.512 30.948 46.448 30.948 46.448 C 30.948 46.448 31 46.5 31 46.5 C 31 46.5 31 -46.5 31 -46.5 C 31 -46.5 -31 -46.5 -31 -46.5 C -31 -46.5 -31 46.5 -31 46.5 " }
            }
            Connections { target: _qt_node0.animations; function onRestart() {_qt_shapePath_0_trim_start_animation.restart() } }
            ParallelAnimation {
                id: _qt_shapePath_0_trim_start_animation
                loops: _qt_node0.animations.loops
                paused: _qt_node0.animations.paused
                running: true
                onLoopsChanged: { if (running) { restart() } }
                SequentialAnimation {
                    SequentialAnimation {
                        loops: 1
                        ScriptAction {
                            script:_qt_shapePath_0.trim.start = 0
                        }
                        PauseAnimation { duration: 63 }
                        PropertyAnimation {
                            duration: 468
                            target: _qt_shapePath_0.trim
                            property: "start"
                            to: 1
                            easing.type: Easing.BezierSpline
                            easing.bezierCurve: [ 0.167, 0, 0.833, 1, 1, 1 ]
                        }
                        PauseAnimation { duration: 500 }
                    }
                }
            }
        }
    }
}
