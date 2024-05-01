
#include "subject.h"

void initSubject(tire::Render *rndr) {
    auto box_one = std::make_shared<tire::Node<float>>();
    box_one->setVerteciesArray({ tire::point3f{ 1.0f, 1.0f, 1.0f },
                                 tire::point3f{ 1.0f, -1.0f, 1.0f },
                                 tire::point3f{ -1.0f, -1.0f, 1.0f },
                                 tire::point3f{ -1.0f, 1.0f, 1.0f },
                                 tire::point3f{ 1.0f, 1.0f, -1.0f },
                                 tire::point3f{ 1.0f, -1.0f, -1.0f },
                                 tire::point3f{ -1.0f, -1.0f, -1.0f },
                                 tire::point3f{ -1.0f, 1.0f, -1.0f } });

    box_one->setIndicesArray({ 0, 1, 2, 3, 0, 2, 4, 5, 6, 7, 4, 6 });
    
    rndr->appendToRenderList(box_one);
}
