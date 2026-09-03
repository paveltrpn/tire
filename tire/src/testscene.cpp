module;

#include <memory>

export module barewindow : testscene;

import graph;

auto testScene() -> std::shared_ptr<tire::Scene> {
    return std::make_shared<tire::Scene>();
}
