
#include <memory>
#include "subject.h"

#include "geometry/node.h"
#include "geometry/polytope.h"

void initSubject( tire::Render *rndr ) {
    auto box_one = std::make_shared<tire::Node>( tire::Box() );
    rndr->appendToRenderList( box_one );
}
