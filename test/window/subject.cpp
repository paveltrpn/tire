
#include <memory>
#include "subject.h"

#include "geometry/node.h"

void initSubject( tire::Render *rndr ) {
    auto box_one = std::make_shared<tire::Node>();

    rndr->appendToRenderList( box_one );
}
