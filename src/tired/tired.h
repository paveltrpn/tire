
#pragma once

#include <vsg/all.h>

#include <QObject>

#include "qt_window/window.h"

namespace tired {

struct Tired final : QObject {
    Q_OBJECT
public:
    Tired( QObject* parent = nullptr );

    auto loadTestScene() -> void;
    auto initCamera( Window* window, uint32_t width, uint32_t height ) -> void;
    auto viewerCompile( int interval, bool continuousUpdate ) -> void;

    auto viewer() -> vsg::ref_ptr<Viewer>;
    auto rootNode() -> vsg::ref_ptr<vsg::Node>;
    auto camera() -> vsg::ref_ptr<vsg::Camera>;

    Q_INVOKABLE void addExBox( float px, float py, float pz, float rx, float ry, float rz, float sx, float sy,
                               float sz );

private:
    auto addTexturePipelineNode() -> void;

private:
    vsg::ref_ptr<Viewer> viewer_{};
    vsg::ref_ptr<vsg::Camera> camera_{};
    vsg::ref_ptr<vsg::Trackball> trackball_{};

    vsg::ref_ptr<vsg::Group> theRoot_{};
    vsg::ref_ptr<vsg::MatrixTransform> texturePipelineNode_{};

    vsg::ref_ptr<vsg::Node> testScene_{};
};

}  // namespace tired
