
#include <print>

#include <vsg/all.h>

#include "../box/ex_box.h"
#include "BasemeshSubgraph.h"

namespace tired {

BasemeshSubgraph::BasemeshSubgraph( QObject *parent )
    : Subgraph{ parent } {
}

auto BasemeshSubgraph::initPipeline() -> void {
    // load shaders
    vsg::Paths searchPaths = std::vector<vsg::Path>{ "/mnt/main/sources/vsgExamples/data" };

    vsg::ref_ptr<vsg::ShaderStage> vertexShader = vsg::ShaderStage::read(
        VK_SHADER_STAGE_VERTEX_BIT, "main", vsg::findFile( "shaders/vert_PushConstants.spv", searchPaths ) );
    vsg::ref_ptr<vsg::ShaderStage> fragmentShader = vsg::ShaderStage::read(
        VK_SHADER_STAGE_FRAGMENT_BIT, "main", vsg::findFile( "shaders/frag_PushConstants.spv", searchPaths ) );
    if ( !vertexShader || !fragmentShader ) {
        std::println( "Could not create shaders." );
        std::terminate();
    }

    // read texture image
    vsg::Path textureFile( "textures/lz.vsgb" );
    auto textureData = vsg::read_cast<vsg::Data>( vsg::findFile( textureFile, searchPaths ) );
    if ( !textureData ) {
        std::println( "Could not read texture file : {} ", textureFile );
        std::terminate();
    }

    // set up graphics pipeline
    vsg::DescriptorSetLayoutBindings descriptorBindings{
        { 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT,
          nullptr }  // { binding, descriptorType, descriptorCount, stageFlags, pImmutableSamplers}
    };

    auto descriptorSetLayout = vsg::DescriptorSetLayout::create( descriptorBindings );

    vsg::PushConstantRanges pushConstantRanges{
        { VK_SHADER_STAGE_VERTEX_BIT, 0,
          128 }  // projection, view, and model matrices, actual push constant calls automatically provided by the VSG's RecordTraversal
    };

    vsg::VertexInputState::Bindings vertexBindingsDescriptions{
        VkVertexInputBindingDescription{ 0, sizeof( vsg::vec3 ), VK_VERTEX_INPUT_RATE_VERTEX },  // vertex data
        VkVertexInputBindingDescription{ 1, sizeof( vsg::vec3 ), VK_VERTEX_INPUT_RATE_VERTEX },  // colour data
        VkVertexInputBindingDescription{ 2, sizeof( vsg::vec2 ), VK_VERTEX_INPUT_RATE_VERTEX }   // tex coord data
    };

    vsg::VertexInputState::Attributes vertexAttributeDescriptions{
        VkVertexInputAttributeDescription{ 0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0 },  // vertex data
        VkVertexInputAttributeDescription{ 1, 1, VK_FORMAT_R32G32B32_SFLOAT, 0 },  // colour data
        VkVertexInputAttributeDescription{ 2, 2, VK_FORMAT_R32G32_SFLOAT, 0 }      // tex coord data
    };

    vsg::GraphicsPipelineStates pipelineStates{
        vsg::VertexInputState::create( vertexBindingsDescriptions, vertexAttributeDescriptions ),
        vsg::InputAssemblyState::create(),
        vsg::RasterizationState::create(),
        vsg::MultisampleState::create(),
        vsg::ColorBlendState::create(),
        vsg::DepthStencilState::create() };

    auto pipelineLayout =
        vsg::PipelineLayout::create( vsg::DescriptorSetLayouts{ descriptorSetLayout }, pushConstantRanges );
    auto graphicsPipeline = vsg::GraphicsPipeline::create(
        pipelineLayout, vsg::ShaderStages{ vertexShader, fragmentShader }, pipelineStates );
    auto bindGraphicsPipeline = vsg::BindGraphicsPipeline::create( graphicsPipeline );

    // create texture image and associated DescriptorSets and binding
    auto texture = vsg::DescriptorImage::create( vsg::Sampler::create(), textureData, 0, 0,
                                                 VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER );

    auto descriptorSet = vsg::DescriptorSet::create( descriptorSetLayout, vsg::Descriptors{ texture } );
    auto bindDescriptorSet =
        vsg::BindDescriptorSet::create( VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, descriptorSet );

    stateGroup_->add( bindGraphicsPipeline );
    stateGroup_->add( bindDescriptorSet );

    // set up model transformation node
    baseNode_ = vsg::MatrixTransform::create();  // VK_SHADER_STAGE_VERTEX_BIT

    stateGroup_->addChild( baseNode_ );
}

auto BasemeshSubgraph::addChild( vsg::ref_ptr<vsg::Node> node ) -> void {
    baseNode_->addChild( node );
}

void BasemeshSubgraph::addExBox( float px, float py, float pz, float rx, float ry, float rz, float sx, float sy,
                                 float sz ) {
    auto exbox = vsg::ref_ptr<ExBox>(
        new ExBox{ vsg::dvec3{ px, py, pz }, vsg::dvec3{ rx, ry, rz }, vsg::dvec3{ sx, sy, sz } } );
    addChild( exbox );

    emit nodeAdded();
}

}  // namespace tired
