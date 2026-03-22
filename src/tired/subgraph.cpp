
#include "subgraph.h"

namespace tired {

Subgraph::Subgraph( vsg::Viewer* viewer, QObject* parent )
    : QObject{ parent }
    , _viewer{ viewer }
    , stateGroup_{ vsg::StateGroup::create() }
    , baseNode_{ vsg::Group::create() } {
    stateGroup_->addChild( baseNode_ );
}

auto Subgraph::stateGroup() -> vsg::ref_ptr<vsg::StateGroup> {
    return stateGroup_;
}

auto Subgraph::link( std::shared_ptr<SceneObjectBase> object ) -> void {
    baseNode_->addChild( object->root() );
    _objectsList.push_back( std::move( object ) );

    recompile();
}

auto Subgraph::recompile() -> void {
    _viewer->compileManager->compile( stateGroup() );
    vsg::CompileResult res{};
    vsg::updateViewer( *_viewer, res );
}

}  // namespace tired
