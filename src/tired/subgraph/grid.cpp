
#include <print>

#include "grid.h"

namespace tired {

Grid::Grid( vsg::Viewer* viewer, QObject* parent )
    : QObject{ parent }
    , _grid{ new GridSubgraph{ viewer } } {
    //
    _grid->initPipeline();
    _grid->initDrawCommand();
}

auto Grid::grid() const -> vsg::ref_ptr<GridSubgraph> {
    return _grid;
}

// ===============================================================================

GridSubgraph::GridSubgraph( vsg::Viewer* viewer )
    : Subgraph{ viewer } {
}

auto GridSubgraph::initPipeline() -> void {
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
        { /* binding */ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, /* count */ 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr },
        { /* binding */ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, /* count */ 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr } };

    auto descriptorSetLayout = vsg::DescriptorSetLayout::create( descriptorBindings );

    auto planeBufUniformValue =
        vsg::vec4Array::create( { vsg::vec4( 10.0, -10.0, 0.0, 1.0 ), vsg::vec4( 10.0, 10.0, 0.0, 1.0 ),
                                  vsg::vec4( -10.0, 10.0, 0.0, 1.0 ), vsg::vec4( -10.0, 10.0, 0.0, 1.0 ),
                                  vsg::vec4( -10.0, -10.0, 0.0, 1.0 ), vsg::vec4( 10.0, -10.0, 0.0, 1.0 ) } );

    auto planeBufUniformDescriptor =
        vsg::DescriptorBuffer::create( planeBufUniformValue, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER );

    _gridBufUniformValue =
        vsg::floatArray::create( { _gridSize, _lineThickness, _maxRange, _zoomSensitivity, _colorMajor.r, _colorMajor.g,
                                   _colorMajor.b, 1.0, _colorMinor.r, _colorMinor.g, _colorMinor.b, _majorDivisor } );

    auto gridBufUniformDescriptor =
        vsg::DescriptorBuffer::create( _gridBufUniformValue, 0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER );

    vsg::GraphicsPipelineStates pipelineStates{ vsg::VertexInputState::create(),   vsg::InputAssemblyState::create(),
                                                vsg::RasterizationState::create(), vsg::MultisampleState::create(),
                                                vsg::ColorBlendState::create(),    vsg::DepthStencilState::create() };

    vsg::PushConstantRanges pushConstantRanges{};

    auto pipelineLayout =
        vsg::PipelineLayout::create( vsg::DescriptorSetLayouts{ descriptorSetLayout }, pushConstantRanges );
    auto graphicsPipeline = vsg::GraphicsPipeline::create(
        pipelineLayout, vsg::ShaderStages{ vertexShader, fragmentShader }, pipelineStates );
    auto bindGraphicsPipeline = vsg::BindGraphicsPipeline::create( graphicsPipeline );

    auto descriptorSet = vsg::DescriptorSet::create(
        descriptorSetLayout, vsg::Descriptors{ planeBufUniformDescriptor, gridBufUniformDescriptor } );

    auto bindDescriptorSet = vsg::BindDescriptorSet::create( VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout,
                                                             /* in_firstSet */ 0, descriptorSet );

    _stateGroup->add( bindGraphicsPipeline );
    _stateGroup->add( bindDescriptorSet );
}

auto GridSubgraph::initDrawCommand() -> void {
    const auto commands = vsg::Commands::create();

    const auto drawCmd = vsg::Draw::create( 6, 1, 0, 0 );
    commands->addChild( drawCmd );

    _stateGroup->addChild( commands );
}

}  // namespace tired
