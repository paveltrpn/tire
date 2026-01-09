
module;

#include <memory>

export module render:ui;

import ui;
import context;

import :pipeline_ui;
import :vertex_buffer;

namespace tire {

struct UiVK final : tire::Ui {
    UiVK( const Context *context )
        : context_{ context } {
        //
        // pipeline_ = std::make_unique<PipelineUi>( context_ );
        // auto testBoxProgram = Program{ context_ };
        // testBoxProgram.fill(
        //   { { vk_simple_box_VERTEX, vertex_stage_suffix }, { vk_simple_box_FRAGMENT, fragment_stage_suffix } } );
        // pipeline_->buildPipeline( testBoxProgram );
    }

    auto flush() -> void override {
        //

        componentsList_.clear();
    }

private:
    const Context *context_;
    std::shared_ptr<PipelineUi> pipeline_{};

    std::shared_ptr<VertexBuffer> verteciesBuffer_;
    std::shared_ptr<VertexBuffer> colorsBuffer_;
    std::shared_ptr<VertexBuffer> texCrdsBuffer_;
};

}  // namespace tire