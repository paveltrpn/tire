
#include "subgraph.h"

namespace tired {

Subgraph::Subgraph( vsg::Viewer* viewer )
    : _viewer{ viewer }
    , _stateGroup{ vsg::StateGroup::create() }
    , _baseNode{ vsg::Group::create() } {
    _stateGroup->addChild( _baseNode );
    //
    this->addChild( _stateGroup );
}

auto Subgraph::link( std::shared_ptr<SceneObjectBase> object ) -> void {
    _baseNode->addChild( object->root() );
    recompile();
}

auto Subgraph::recompile() -> void {
    _viewer->compileManager->compile( _stateGroup );
    vsg::CompileResult res{};
    vsg::updateViewer( *_viewer, res );
}

}  // namespace tired
