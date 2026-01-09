
module;

#include <memory>

export module render:ui;

import ui;
import context;

import :pipeline_ui;
import :vertex_buffer;

namespace tire {

struct UiVK final : tire::Ui {
    auto flush() -> void override {
        //

        componentsList_.clear();
    }

private:
    const Context *context_;
    std::unique_ptr<PipelineUi> pipeline_{};

    VertexBuffer verteciesBuffer_;
    VertexBuffer colorsBuffer_;
    VertexBuffer texCrdsBuffer_;
};

}  // namespace tire