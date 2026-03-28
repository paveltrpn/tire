
#include "subgraph.h"

namespace tired {

Subgraph::Subgraph( vsg::Viewer* viewer )
    : _viewer{ viewer }
    , stateGroup_{ vsg::StateGroup::create() } {
    this->addChild( stateGroup_ );
}

auto Subgraph::recompile() -> void {
    _viewer->compileManager->compile( stateGroup_ );
    vsg::CompileResult res{};
    vsg::updateViewer( *_viewer, res );
}

}  // namespace tired
