
#include "subject.h"

void initSubject(tire::Render *rndr) {
    auto box_one = std::make_shared<tire::Node<float>>();

    box_one->setVerteciesArray(
      { tire::point3f{ 1.0f, 1.0f, 1.0f },   tire::point3f{ -1.0f, 1.0f, 1.0f },
        tire::point3f{ -1.0f, -1.0f, 1.0f }, tire::point3f{ 1.0f, 1.0f, 1.0f },
        tire::point3f{ -1.0f, -1.0f, 1.0f }, tire::point3f{ 1.0f, -1.0f, 1.0f },
        tire::point3f{ 1.0f, -1.0f, -1.0f }, tire::point3f{ 1.0f, -1.0f, 1.0f },
        tire::point3f{ -1.0f, -1.0f, 1.0f }, tire::point3f{ 1.0f, -1.0f, -1.0f },
        tire::point3f{ -1.0f, -1.0f, 1.0f }, tire::point3f{ -1.0f, -1.0f, -1.0f },
        tire::point3f{ -1.0f, -1.0, -1.0 },  tire::point3f{ -1.0f, -1.0, 1.0 },
        tire::point3f{ -1.0f, 1.0, 1.0 },    tire::point3f{ -1.0f, -1.0f, -1.0 },
        tire::point3f{ -1.0f, 1.0f, 1.0 },   tire::point3f{ -1.0f, 1.0f, -1.0 },
        tire::point3f{ -1.0f, 1.0f, -1.0f }, tire::point3f{ 1.0f, 1.0f, -1.0f },
        tire::point3f{ 1.0f, -1.0f, -1.0f }, tire::point3f{ -1.0f, 1.0f, -1.0 },
        tire::point3f{ 1.0f, -1.0f, -1.0 },  tire::point3f{ -1.0f, -1.0f, -1.0 },
        tire::point3f{ 1.0f, 1.0f, -1.0 },   tire::point3f{ 1.0f, 1.0f, 1.0 },
        tire::point3f{ 1.0f, -1.0f, 1.0 },   tire::point3f{ 1.0f, 1.0f, -1.0 },
        tire::point3f{ 1.0f, -1.0f, 1.0 },   tire::point3f{ 1.0f, -1.0f, -1.0 },
        tire::point3f{ -1.0f, 1.0f, -1.0f }, tire::point3f{ -1.0f, 1.0f, 1.0f },
        tire::point3f{ 1.0f, 1.0f, 1.0f },   tire::point3f{ -1.0f, 1.0, -1.0f },
        tire::point3f{ 1.0f, 1.0, 1.0f },    tire::point3f{ 1.0f, 1.0, -1.0f } });

    rndr->appendToRenderList(box_one);
}
