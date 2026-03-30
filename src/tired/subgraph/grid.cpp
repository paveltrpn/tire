
#include <print>

#include "grid.h"

namespace tired {

Grid::Grid( vsg::Viewer* viewer )
    : Subgraph{ viewer } {
}

auto Grid::initPipeline() -> void {
    // load shaders
    vsg::Paths searchPaths =
        std::vector<vsg::Path>{ "/mnt/main/code/tire_ed/shaders/spirv", "/mnt/main/code/tire_ed/assets" };

    vsg::ref_ptr<vsg::ShaderStage> vertexShader =
        vsg::ShaderStage::read( VK_SHADER_STAGE_VERTEX_BIT, "main", vsg::findFile( "vert_grid.spv", searchPaths ) );
    vsg::ref_ptr<vsg::ShaderStage> fragmentShader =
        vsg::ShaderStage::read( VK_SHADER_STAGE_FRAGMENT_BIT, "main", vsg::findFile( "frag_grid.spv", searchPaths ) );
    if ( !vertexShader || !fragmentShader ) {
        std::println( "Could not create shaders." );
        std::terminate();
    }

    // set up graphics pipeline
    vsg::DescriptorSetLayoutBindings descriptorBindings{
        { 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr } };

    auto descriptorSetLayout = vsg::DescriptorSetLayout::create( descriptorBindings );

    const auto gridSize{ 0.5f };
    const auto lineThickness{ 0.015f };
    const auto maxRange{ 100.0f };
    const auto zoomSensitivity{ 0.05f };
    const auto colorMajor = vsg::vec3{ 0.7f, 0.2f, 0.2f };
    const auto colorMinor = vsg::vec3{ 0.2f, 0.2f, 0.5f };
    const auto majorDivisor = 5.0f;

    auto gridBufUniformValue =
        vsg::floatArray::create( { gridSize, lineThickness, maxRange, zoomSensitivity, colorMajor.r, colorMajor.g,
                                   colorMajor.b, 1.0, colorMinor.r, colorMinor.g, colorMinor.b, majorDivisor } );

    auto gridBufUniformDescriptor =
        vsg::DescriptorBuffer::create( gridBufUniformValue, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER );

    vsg::VertexInputState::Bindings vertexBindingsDescriptions{};

    vsg::VertexInputState::Attributes vertexAttributeDescriptions{};

    vsg::GraphicsPipelineStates pipelineStates{
        vsg::VertexInputState::create( vertexBindingsDescriptions, vertexAttributeDescriptions ),
        vsg::InputAssemblyState::create(),
        vsg::RasterizationState::create(),
        vsg::MultisampleState::create(),
        vsg::ColorBlendState::create(),
        vsg::DepthStencilState::create() };

    vsg::PushConstantRanges pushConstantRanges{};

    auto pipelineLayout =
        vsg::PipelineLayout::create( vsg::DescriptorSetLayouts{ descriptorSetLayout }, pushConstantRanges );
    auto graphicsPipeline = vsg::GraphicsPipeline::create(
        pipelineLayout, vsg::ShaderStages{ vertexShader, fragmentShader }, pipelineStates );
    auto bindGraphicsPipeline = vsg::BindGraphicsPipeline::create( graphicsPipeline );

    auto descriptorSet =
        vsg::DescriptorSet::create( descriptorSetLayout, vsg::Descriptors{ gridBufUniformDescriptor } );

    auto bindDescriptorSet =
        vsg::BindDescriptorSet::create( VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, descriptorSet );

    _stateGroup->add( bindGraphicsPipeline );
    _stateGroup->add( bindDescriptorSet );
}

auto Grid::initDrawCommand() -> void {
    const auto commands = vsg::Commands::create();

    const auto drawCmd = vsg::Draw::create( 6, 1, 0, 0 );
    commands->addChild( drawCmd );

    const auto tr = vsg::MatrixTransform::create();
    tr->addChild( commands );

    _stateGroup->addChild( tr );
}

}  // namespace tired
