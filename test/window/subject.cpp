
#include <memory>
#include "subject.h"

import geometry;

void initSubject( tire::Render *rndr ) {
    auto box_one = std::make_shared<tire::Node>();

    rndr->appendToRenderList( box_one );
}
