
#pragma once

#include <vsg/all.h>

#include <QObject>

#include <vsgQt/Window.h>

namespace tired {

struct Tired final : QObject {
    Q_OBJECT
public:
    Tired( QObject* parent = nullptr );

    auto loadTestScene( vsg::Path filename, vsg::ref_ptr<vsg::Options> options ) -> int;
    auto viewerCompile( int interval, bool continuousUpdate ) -> void;

    auto viewer() -> vsg::ref_ptr<vsgQt::Viewer>;
    auto rootNode() -> vsg::ref_ptr<vsg::Node>;

private:
    vsg::ref_ptr<vsgQt::Viewer> viewer_{};
    vsg::ref_ptr<vsg::Group> theRoot_{};

    vsg::ref_ptr<vsg::Node> testScene_{};
};

}  // namespace tired
