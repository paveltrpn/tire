
#include <memory>
#include "scene.h"

#include "geometry/node.h"
#include "geometry/polytope.h"

void initScene( tire::Render *rndr ) {
    auto box_one = std::make_shared<tire::Node>( tire::Box() );
    rndr->appendToRenderList( box_one );
}
