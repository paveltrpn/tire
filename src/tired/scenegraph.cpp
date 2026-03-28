
#include "scenegraph.h"
#include "scene_object/sceneobjectdata.h"
#include "scene_object/box.h"

namespace tired {

Scenegraph::Scenegraph( vsg::Viewer* viewer )
    : _root{ new vsg::Group{} }
    , _basemeshSubgraph{ new BasemeshSubgraph{ viewer } }
    , _obstaclesSubgraph{ new ObstaclesSubgraph{ viewer } } {
    _root->addChild( _basemeshSubgraph );
    _root->addChild( _obstaclesSubgraph );
}

auto Scenegraph::root() -> vsg::ref_ptr<vsg::Group> {
    return _root;
}

auto Scenegraph::linkToBasemesh( std::shared_ptr<SceneObjectBase> object ) -> void {
    _basemeshSubgraph->addChild( object->root() );
    _basemeshSubgraph->recompile();
    _objectsList.push_back( std::move( object ) );
}

auto Scenegraph::linkToObstacles( std::shared_ptr<SceneObjectBase> object ) -> void {
    _obstaclesSubgraph->addChild( object->root() );
    _obstaclesSubgraph->recompile();
    _objectsList.push_back( std::move( object ) );
}

void Scenegraph::addExBox( float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz ) {
    auto data = BoxObjectData{};
    data.setPosition( { px, py, pz } );
    data.setOrientation( { rx, ry, rz } );
    data.setScale( { sx, sy, sz } );

    auto exbox = std::make_shared<object::Box>( data );

    linkToBasemesh( exbox );
}

}  // namespace tired
