
#include "scenegraph.h"
#include "scene_object/sceneobjectdata.h"
#include "scene_object/box.h"

namespace tired {

Scenegraph::Scenegraph( vsg::Viewer* viewer, QObject* parent )
    : QObject{ parent }
    , _root{ new vsg::Group{} }
    , _viewer{ viewer }
    , _testbox{ new Testbox{ viewer } }
    , _grid{ new Grid{ viewer } }
    , _basemeshSubgraph{ new BasemeshSubgraph{ viewer } }
    , _obstaclesSubgraph{ new ObstaclesSubgraph{ viewer } } {
    //

    _testbox->initPipeline();
    _grid->initPipeline();
    _basemeshSubgraph->initPipeline();
    _obstaclesSubgraph->initPipeline();

    _root->addChild( _testbox );
    _root->addChild( _grid );
    _root->addChild( _basemeshSubgraph );
    _root->addChild( _obstaclesSubgraph );
}

auto Scenegraph::root() -> vsg::ref_ptr<vsg::Group> {
    return _root;
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

auto Scenegraph::addTestBox() -> void {
    _testbox->initDrawCommand();

    _viewer->compileManager->compile( _testbox );
    vsg::CompileResult res{};
    vsg::updateViewer( *_viewer, res );
}

}  // namespace tired
