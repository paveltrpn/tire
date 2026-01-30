
#pragma once

#include <vsg/all.h>

#include <QObject>

#include <vsgQt/Window.h>

namespace tired {

struct Tired final : QObject {
    Q_OBJECT
public:
    Tired( QObject* parent = nullptr );

    auto loadTestScene() -> void;
    auto initWindow( vsg::ref_ptr<vsg::WindowTraits> traits, QWindow* parent ) -> vsgQt::Window*;
    auto viewerCompile( int interval, bool continuousUpdate ) -> void;

    auto viewer() -> vsg::ref_ptr<vsgQt::Viewer>;
    auto rootNode() -> vsg::ref_ptr<vsg::Node>;
    auto camera() -> vsg::ref_ptr<vsg::Camera>;

    Q_INVOKABLE void setCameraToBounds();

    auto addCustomPipelineNode() -> void;

private:
    vsg::ref_ptr<vsgQt::Viewer> viewer_{};
    vsg::ref_ptr<vsg::Camera> camera_{};
    vsg::ref_ptr<vsg::Trackball> trackball_{};

    vsg::ref_ptr<vsg::Group> theRoot_{};
    vsg::ref_ptr<vsg::MatrixTransform> costomPipelineNode_{};

    vsg::ref_ptr<vsg::Node> testScene_{};
};

}  // namespace tired
