
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
    , _sceneObjectSubgraph{ new SceneObjectSubgraph{ viewer } }
    , _markerSubgraph{ new MarkerSubgraph{ viewer } } {
    //

    // Draw red box.
    _testbox->initPipeline();
    _testbox->initDrawCommand();

    _sceneObjectSubgraph->initPipeline();
    _markerSubgraph->initPipeline();

    // Enable test box.
    _root->addChild( _testbox );

    _root->addChild( _grid->grid() );

    _root->addChild( _sceneObjectSubgraph );
    _root->addChild( _markerSubgraph );
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

    _sceneObjectSubgraph->link( exbox );
    _objectsList.push_back( std::move( exbox ) );
}

}  // namespace tired
