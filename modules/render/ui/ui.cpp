
module;

#include <memory>
#include <filesystem>

export module render:ui;

import ui;
import context;
import config;

import :pipeline_ui;
import :vertex_buffer;

namespace tire {

struct UiVK final : tire::Ui {
    UiVK( const Context *context )
        : context_{ context } {
        //

        const auto configHandle = Config::instance();
        const auto basePath = configHandle->getBasePath().string();

        pipeline_ = std::make_unique<PipelineUi>( context_ );

        auto program = Program{ context_ };
        program.fill( {
          //
          basePath + "/shaders/spirv/vk_ui_VERTEX.spv",
          basePath + "/shaders/spirv/vk_ui_FRAGMENT.spv",
        } );

        pipeline_->buildPipeline( program );
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