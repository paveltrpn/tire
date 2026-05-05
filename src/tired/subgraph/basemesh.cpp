
#include <print>

#include <vsg/all.h>

#include "image/tga.h"
#include "log/log.h"
#include "basemesh.h"

namespace tired {

BasemeshSubgraph::BasemeshSubgraph( vsg::Viewer* viewer )
    : Subgraph{ viewer } {
}

auto BasemeshSubgraph::initPipeline() -> void {
    // load shaders
    vsg::Paths searchPaths =
        std::vector<vsg::Path>{ "/mnt/main/code/tire_ed/shaders/spirv", "/mnt/main/code/tire_ed/assets" };

    vsg::ref_ptr<vsg::ShaderStage> vertexShader = vsg::ShaderStage::read(
        VK_SHADER_STAGE_VERTEX_BIT, "main", vsg::findFile( "vert_PushConstants.spv", searchPaths ) );
    vsg::ref_ptr<vsg::ShaderStage> fragmentShader = vsg::ShaderStage::read(
        VK_SHADER_STAGE_FRAGMENT_BIT, "main", vsg::findFile( "frag_PushConstants.spv", searchPaths ) );
    if ( !vertexShader || !fragmentShader ) {
        std::println( "Could not create shaders." );
        std::terminate();
    }

    // read texture image
    vsg::Path textureFile( "textures/lz.vsgb" );
    auto textureData1 = vsg::read_cast<vsg::Data>( vsg::findFile( textureFile, searchPaths ) );
    if ( !textureData1 ) {
        std::println( "Could not read texture file : {} ", textureFile );
        std::terminate();
    }

    // read texture image
    auto rawData = std::make_unique<Tga>( "/mnt/main/code/tire_ed/assets/textures/Onyx006_color.tga" );
    auto rawDataSize = rawData->width() * rawData->height() * rawData->components();

    auto vsgData = vsg::ubyteArray::create( rawDataSize );
    std::memcpy( vsgData->dataPointer(), rawData->data(), rawDataSize );

    auto imageData = vsg::ubyteArray2D::create( rawData->width(), rawData->height(), vsgData->data() );
    imageData->properties.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageData->properties.stride = 0;
    imageData->properties.mipLevels = 0;
    imageData->properties.blockWidth = 1;
    imageData->properties.blockHeight = 1;
    imageData->properties.blockDepth = 1;
    imageData->properties.origin = vsg::Origin::TOP_LEFT;
    imageData->properties.imageViewType = -1;
    imageData->properties.dataVariance = vsg::DataVariance::STATIC_DATA;
    imageData->properties.allocatorType = vsg::AllocatorType::ALLOCATOR_TYPE_VSG_ALLOCATOR;

    auto textureImage = vsg::Image::create( imageData );
    auto d = dynamic_cast<vsg::Data*>( textureImage.get() );
    if ( !d ) {
        log::error()( " ===== bad cast" );
    }
    auto textureData = vsg::ref_ptr<vsg::Data>( d );

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
    auto sampler = vsg::Sampler::create();
    sampler->magFilter = VK_FILTER_LINEAR;
    sampler->minFilter = VK_FILTER_LINEAR;
    sampler->mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    auto texture =
        vsg::DescriptorImage::create( sampler, textureData1, 0, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER );

    auto descriptorSet = vsg::DescriptorSet::create( descriptorSetLayout, vsg::Descriptors{ texture } );
    auto bindDescriptorSet =
        vsg::BindDescriptorSet::create( VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, descriptorSet );

    _stateGroup->add( bindGraphicsPipeline );
    _stateGroup->add( bindDescriptorSet );
}

}  // namespace tired
