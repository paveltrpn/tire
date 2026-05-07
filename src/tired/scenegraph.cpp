
#include "scenegraph.h"
#include "scene_object/sceneobjectdata.h"
#include "scene_object/box.h"

namespace tired {

Scenegraph::Scenegraph( vsg::Viewer* viewer, QObject* parent )
    : QObject{ parent }
    , _root{ new vsg::Group{} }
    , _viewer{ viewer }
    , _testbox{ new Testbox{ viewer } }
    , _grid{ new Grid{ viewer, this } }
    , _basemeshSubgraph{ new SceneObjectSubgraph{ viewer } }
    , _obstaclesSubgraph{ new ObstaclesSubgraph{ viewer } } {
    //

    // Draw red box.
    _testbox->initPipeline();
    _testbox->initDrawCommand();

    _basemeshSubgraph->initPipeline();
    _obstaclesSubgraph->initPipeline();

    // Enable test box.
    _root->addChild( _testbox );

    _root->addChild( _grid->grid() );

    _root->addChild( _basemeshSubgraph );
    _root->addChild( _obstaclesSubgraph );
}

auto Scenegraph::root() -> vsg::ref_ptr<vsg::Group> {
    return _root;
}

auto Scenegraph::grid() const -> Grid* {
    return _grid;
}

void Scenegraph::addExBox( float px, float py, float pz, float rx, float ry, float rz, float sx, float sy, float sz ) {
    auto data = BoxObjectData{};
    data.setPosition( { px, py, pz } );
    data.setOrientation( { rx, ry, rz } );
    data.setScale( { sx, sy, sz } );

    auto exbox = std::make_shared<object::Box>( data );

    _basemeshSubgraph->link( exbox );
    _objectsList.push_back( std::move( exbox ) );
}

}  // namespace tired
